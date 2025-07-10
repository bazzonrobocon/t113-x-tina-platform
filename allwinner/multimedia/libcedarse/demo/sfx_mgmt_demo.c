// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * sfx_mgmt_demo.c -- sfx for hal test demo
 *
 * Dby <dby@allwinnertech.com>
 *
 * Copyright (c) 2023 Allwinnertech Ltd.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <endian.h>
#include <pthread.h>
#include <unistd.h>

#include "sfx_log.h"
#include "sfx_mgmt.h"
#include "tinyalsa/asoundlib.h"

/* ctrl c */
static int g_close = 0;
static void stream_close(int sig);

/* audio sfx */
struct sfx_platform *g_platform = NULL;
pthread_t param_adjust_thread = 0;
static void *param_adjust(void *arg);

/* audio play */
#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164

struct riff_wave_header {
    uint32_t riff_id;
    uint32_t riff_sz;
    uint32_t wave_id;
};

struct chunk_header {
    uint32_t id;
    uint32_t sz;
};

struct chunk_fmt {
    uint16_t audio_format;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
};

static int parser_wavhead_to_pcmconfig(FILE *wav_file, uint32_t *wav_sz, struct pcm_config *config);
static struct pcm *sfx_pcm_open(unsigned int card, unsigned int device,
                                unsigned int stream, struct pcm_config *config);

/* parser & help */
enum SFX_DEMO_FUNC {
    SFX_DEMO_DATA = 0,
    SFX_DEMO_PLAY,
    SFX_DEMO_CAP,
    SFX_DEMO_VERSION,

    SFX_DEMO_CNT,
};

struct demo_func_cfg_table {
    char *name;
    enum SFX_DEMO_FUNC func;
} g_func_cfg_table[] = {
    {"data",        SFX_DEMO_DATA},
    {"play",        SFX_DEMO_PLAY},
    {"cap",         SFX_DEMO_CAP},
    {"version",     SFX_DEMO_VERSION},
};

struct sfx_demo_config {
    enum SFX_DEMO_FUNC func;
    const char *dev_name;
    unsigned int per_delay_open;
    unsigned int per_delay_do;
    bool param_adj_func;
    unsigned int loop_cnt;
    char *in_filename;
    char *out_filename;

    unsigned int card;
    unsigned int device;
    unsigned int rate;
    unsigned int channels;
    unsigned int bits;
    unsigned int period_size;
    unsigned int period_count;
    unsigned int cap_time;

    /* misc */
    uint32_t wav_data_sz;
    struct pcm_config wav_pcm_config;
};

void demo_help(char *argv0);
int demo_args_parser(int argc, char **argv, struct sfx_demo_config *demo_cfg);
void demo_args_show(struct sfx_demo_config *demo_cfg);

/* demo function */
int demo_func_data(struct sfx_demo_config *demo_cfg);
int demo_func_play(struct sfx_demo_config *demo_cfg);
int demo_func_cap(struct sfx_demo_config *demo_cfg);
int demo_func_version(struct sfx_demo_config *demo_cfg);

int main(int argc, char **argv)
{
    int ret = 0;
    void *result = NULL;
    struct sfx_demo_config demo_cfg;
    unsigned int loop_num;

    memset(&demo_cfg, 0, sizeof(struct sfx_demo_config));
    if (demo_args_parser(argc, argv, &demo_cfg) < 0) {
        demo_help(argv[0]);
        return 0;
    }
    demo_args_show(&demo_cfg);

    if (demo_cfg.func >= SFX_DEMO_DATA && demo_cfg.func < SFX_DEMO_CNT) {
        /* sfx mgmt init */
        ret = sfxmgmt_init(&g_platform, SFX_MGMT_AUDIO_DEVICE_DESC);
        if (ret) {
            sfx_log_err("sfxmgmt_init failed\n");
            ret = -1;
            goto err;
        }
    } else {
        return 0;
    }

    if (demo_cfg.param_adj_func) {
        /* sfx param adjust start */
        if (pthread_create(&param_adjust_thread, NULL, param_adjust, g_platform) != 0) {
            sfx_log_err("Create param adjust thread error!\n");
            goto err;
        }
    }

    for (loop_num = 0; loop_num < demo_cfg.loop_cnt; ++loop_num) {
        if (demo_cfg.loop_cnt > 1)
            sfx_log_info("======*** loop run number: %u ***======\n", loop_num + 1);
        if (demo_cfg.per_delay_open) {
            usleep(demo_cfg.per_delay_open * 1000);
            sfx_log_info("ap open start\n");
        }
        switch (demo_cfg.func) {
        case SFX_DEMO_DATA:
            ret = demo_func_data(&demo_cfg);
        break;
        case SFX_DEMO_PLAY:
            ret = demo_func_play(&demo_cfg);
        break;
        case SFX_DEMO_CAP:
            ret = demo_func_cap(&demo_cfg);
        break;
        case SFX_DEMO_VERSION:
            ret = demo_func_version(&demo_cfg);
        break;
        default:
            return 0;
        }
    }

err:
    if (param_adjust_thread) {
        /* sfx param adjust stop */
        pthread_kill(param_adjust_thread, SIGUSR1);
        pthread_join(param_adjust_thread, &result);
        param_adjust_thread = 0;
    }

    if (g_platform) {
        /* sfx mgmt exit */
        sfxmgmt_exit(g_platform);
        g_platform = NULL;
    }

    /* sfx_log_show("demo test end\n"); */
    return ret;
}

static void stream_close(int sig)
{
    /* allow the stream to be closed gracefully */
    signal(sig, SIG_IGN);
    g_close = 1;
}

void param_thread_exit_handler()
{
    pthread_exit(NULL);
}

static void *param_adjust(void *arg)
{
    struct sfx_platform *platform = (struct sfx_platform *)arg;

    struct sigaction actions;
    memset(&actions, 0, sizeof(actions));
    sigemptyset(&actions.sa_mask);
    actions.sa_flags = 0;
    actions.sa_handler = param_thread_exit_handler;
    sigaction(SIGUSR1, &actions, NULL);

    /* set anther thread can cancel this thread */
    pthread_detach(pthread_self());

    sfxmgmt_parm_adjust_server(platform);

    pthread_exit(NULL);
}

static int parser_wavhead_to_pcmconfig(FILE *wav_file, uint32_t *wav_sz, struct pcm_config *config)
{
    struct riff_wave_header riff_wave_header;
    struct chunk_header chunk_header;
    struct chunk_fmt chunk_fmt;
    int more_chunks = 1;

    if (!wav_file || !config) {
        sfx_log_err("wav_file or pcm_config invalid\n");
        return -1;
    }

    if (fread(&riff_wave_header, sizeof(riff_wave_header), 1, wav_file) != 1) {
        sfx_log_err("fread failed\n");
        return -1;
    }
    if ((riff_wave_header.riff_id != ID_RIFF) || (riff_wave_header.wave_id != ID_WAVE)) {
        sfx_log_err("wav file is not a riff/wave file\n");
        return -1;
    }
    do {
        if (fread(&chunk_header, sizeof(chunk_header), 1, wav_file) != 1) {
            sfx_log_err("fread failed\n");
            return -1;
        }

        switch (chunk_header.id) {
        case ID_FMT:
            if (fread(&chunk_fmt, sizeof(chunk_fmt), 1, wav_file) != 1) {
                sfx_log_err("fread failed\n");
                return -1;
            }
            /* If the format header is larger, skip the rest */
            if (chunk_header.sz > sizeof(chunk_fmt))
                fseek(wav_file, chunk_header.sz - sizeof(chunk_fmt), SEEK_CUR);
            break;
        case ID_DATA:
            /* Stop looking for chunks */
            more_chunks = 0;
            chunk_header.sz = le32toh(chunk_header.sz);
            break;
        default:
            /* Unknown chunk, skip bytes */
            fseek(wav_file, chunk_header.sz, SEEK_CUR);
        }
    } while (more_chunks);

    memset(config, 0, sizeof(struct pcm_config));
    config->channels    = chunk_fmt.num_channels;
    config->rate        = chunk_fmt.sample_rate;
    if (chunk_fmt.bits_per_sample == 32)
        config->format  = PCM_FORMAT_S32_LE;
    else if (chunk_fmt.bits_per_sample == 24)
        config->format  = PCM_FORMAT_S24_3LE;
    else if (chunk_fmt.bits_per_sample == 16)
        config->format  = PCM_FORMAT_S16_LE;

    *wav_sz = chunk_header.sz;

    return 0;
}

static int check_param(struct pcm_params *params, unsigned int param, unsigned int value,
                       char *param_name, char *param_unit)
{
    unsigned int min;
    unsigned int max;
    int is_within_bounds = 1;

    min = pcm_params_get_min(params, param);
    if (value < min) {
        sfx_log_err("%s is %u%s, device only supports >= %u%s\n",
                    param_name, value, param_unit, min, param_unit);
        is_within_bounds = 0;
    }

    max = pcm_params_get_max(params, param);
    if (value > max) {
        sfx_log_err("%s is %u%s, device only supports <= %u%s\n",
                    param_name, value, param_unit, max, param_unit);
        is_within_bounds = 0;
    }

    return is_within_bounds;
}

static struct pcm *sfx_pcm_open(unsigned int card, unsigned int device, unsigned int stream,
                                struct pcm_config *config)
{
    int can_open;
    struct pcm *pcm;
    struct pcm_params *params;

    params = pcm_params_get(card, device, stream);
    if (params == NULL) {
        sfx_log_err("Unable to open PCM device %u.\n", device);
        return NULL;
    }
    can_open = check_param(params, PCM_PARAM_RATE,
                           config->rate, "Sample rate", "Hz");
    can_open &= check_param(params, PCM_PARAM_CHANNELS,
                           config->channels, "Sample", " channels");
    can_open &= check_param(params, PCM_PARAM_SAMPLE_BITS,
                           pcm_format_to_bits(config->format), "Bitrate", " bits");
    can_open &= check_param(params, PCM_PARAM_PERIOD_SIZE,
                           config->period_size, "Period size", " frames");
    can_open &= check_param(params, PCM_PARAM_PERIODS,
                           config->period_count, "Period count", " periods");
    pcm_params_free(params);
    if (!can_open)
        return NULL;

    pcm = pcm_open(card, device, stream, config);
    if (!pcm || !pcm_is_ready(pcm)) {
        sfx_log_err("Unable to open PCM device %u (%s)\n", device, pcm_get_error(pcm));
        return NULL;
    }

    return pcm;
}

void demo_help(char *argv0)
{
    unsigned int i = 0;
    static struct demo_help {
        const char opt[16];
        const char val[32];
        const char note[128];
    } help[] = {
        {"Opt",  "Val",                 "note"},
        {"-F",   "data",                "demo func: audio data test only"},
        {"",     "play",                "demo func: audio data test with play"},
        {"",     "cap",                 "demo func: audio data test with cap"},
        {"",     "version",             "demo func: get sfxmgmt and sound effect list Verison"},
        {"-DEV", "Speaker",             "sfx dev: Speaker"},
        {"",     "MIC",                 "sfx dev: MIC"},
        {"",     "xxx",                 "sfx dev: xxx(from audio_device_desc.xml play/cap_device)"},
        {"-DLY", "delay open time = #", "sleep #ms per ap open"},
        {"-GAP", "run gap time = #",    "sleep #ms per ap do(only apply data function)"},
        {"-ADJ", "param adj switch",    "param adj function switch, off(0) on(1), fefault off"},
        {"-L",   "loop run number",     "init -> loop(open -> do... -> close) -> exit"},
        {"-i",   "xxx.wav",             "input wav format audio file for play or data"},
        {"-o",   "xxx.pcm",             "output pcm format audio file after algo process"},
        {"",     "",                    "if capture, save xxx.pcm-raw for original data"},

        {"-D",   "card num",            "sound card number"},
        {"-d",   "card device num",     "sound card device number"},
        {"-r",   "sample rate = #",     "capture sample rate"},
        {"-c",   "channels = #",        "capture channels"},
        {"-b",   "sample bits = #",     "capture sample bits"},
        {"-p",   "period size = #",     "Interrupt interval # microseconds"},
        {"-n",   "period count = #",    "# period count"},
        {"-t",   "capture time =  #",   "capture time for test capture only"},
    };
    static unsigned int help_count = sizeof(help) / sizeof(help[0]);

    sfx_log_show("Usage: %s {-F function} [Opt ...]\n", argv0);
    sfx_log_show("%-8s  %-24s  %s\n", help[i].opt, help[i].val, help[i].note);
    for (i = 1; i < help_count; ++i)
        sfx_log_show("%-8s  %-24s  %s\n", help[i].opt, help[i].val, help[i].note);
}

int demo_args_parser(int argc, char **argv, struct sfx_demo_config *demo_cfg)
{
    int ret = 0;
    unsigned int i;
    FILE *in_file = NULL;

    if (argc < 3)
        return -1;

    demo_cfg->func = SFX_DEMO_DATA;
    demo_cfg->dev_name = "Speaker";
    demo_cfg->loop_cnt = 1;
    demo_cfg->in_filename = NULL;
    demo_cfg->out_filename = NULL;
    demo_cfg->card = 0;
    demo_cfg->device = 0;
    demo_cfg->rate = 0;
    demo_cfg->channels = 0;
    demo_cfg->bits = 0;
    demo_cfg->period_size = 1024;
    demo_cfg->period_count = 4;
    demo_cfg->per_delay_open = 0;
    demo_cfg->per_delay_do = 0;
    demo_cfg->cap_time = 0;

    while (*argv) {
        if (strcmp(*argv, "-F") == 0) {
            argv++;
            for (i = 0; i < SFX_DEMO_CNT; ++i) {
                if (strcmp(*argv, g_func_cfg_table[i].name) == 0) {
                    demo_cfg->func = g_func_cfg_table[i].func;
                    break;
                }
            }
            if (i == SFX_DEMO_CNT)
                return -1;
        } else if (strcmp(*argv, "-DEV") == 0) {
            argv++;
            demo_cfg->dev_name = *argv;
        } else if (strcmp(*argv, "-DLY") == 0) {
            argv++;
            if (*argv)
                demo_cfg->per_delay_open = atoi(*argv);
        } else if (strcmp(*argv, "-GAP") == 0) {
            argv++;
            if (*argv)
                demo_cfg->per_delay_do = atoi(*argv);
        } else if (strcmp(*argv, "-ADJ") == 0) {
            argv++;
            if (*argv) {
                if (atoi(*argv))
                    demo_cfg->param_adj_func = true;
                else
                    demo_cfg->param_adj_func = false;
            }
        } else if (strcmp(*argv, "-L") == 0) {
            argv++;
            if (*argv)
                demo_cfg->loop_cnt = atoi(*argv);
        } else if (strcmp(*argv, "-i") == 0) {
            argv++;
            demo_cfg->in_filename = *argv;
        } else if (strcmp(*argv, "-o") == 0) {
            argv++;
            demo_cfg->out_filename = *argv;
        } else if (strcmp(*argv, "-D") == 0) {
            argv++;
            if (*argv)
                demo_cfg->card = atoi(*argv);
        } else if (strcmp(*argv, "-d") == 0) {
            argv++;
            if (*argv)
                demo_cfg->device = atoi(*argv);
        } else if (strcmp(*argv, "-r") == 0) {
            argv++;
            if (*argv)
                demo_cfg->rate = atoi(*argv);
        } else if (strcmp(*argv, "-c") == 0) {
            argv++;
            if (*argv)
                demo_cfg->channels = atoi(*argv);
        } else if (strcmp(*argv, "-b") == 0) {
            argv++;
            if (*argv)
                demo_cfg->bits = atoi(*argv);
        }else if (strcmp(*argv, "-p") == 0) {
            argv++;
            if (*argv)
                demo_cfg->period_size = atoi(*argv);
        } else if (strcmp(*argv, "-n") == 0) {
            argv++;
            if (*argv)
                demo_cfg->period_count = atoi(*argv);
        } else if (strcmp(*argv, "-t") == 0) {
            argv++;
            if (*argv)
                demo_cfg->cap_time = atoi(*argv);
        }
        if (*argv)
            argv++;
    }

    /* note: parser wav head for demo args show only. */
    if (demo_cfg->in_filename) {
        /* wav parser */
        in_file = fopen(demo_cfg->in_filename, "rb");
        if (!in_file) {
            sfx_log_err("Unable to open file(%s)\n", demo_cfg->in_filename);
            return -1;
        }
        ret = parser_wavhead_to_pcmconfig(in_file,
                                          &demo_cfg->wav_data_sz,
                                          &demo_cfg->wav_pcm_config);
        if (ret) {
            sfx_log_err("parser_wavhead_to_pcmconfig failed\n");
            goto err;
        }

        demo_cfg->rate     = demo_cfg->wav_pcm_config.rate;
        demo_cfg->channels = demo_cfg->wav_pcm_config.channels;
        switch (demo_cfg->wav_pcm_config.format) {
        case PCM_FORMAT_S16_LE:
            demo_cfg->bits = 16;
            break;
        case PCM_FORMAT_S24_3LE:
        case PCM_FORMAT_S32_LE:
            demo_cfg->bits = 32;
            break;
        default:
            sfx_log_err("unsupport pcm format\n");
            goto err;
        }
    }

    return 0;

err:
    if (in_file)
        fclose(in_file);
    return -1;
}

void demo_args_show(struct sfx_demo_config *demo_cfg)
{
    unsigned int i;
    char *func_name = NULL;

    if (!demo_cfg) {
        sfx_log_err("demo_cfg invalid\n");
        return;
    }

    for (i = 0; i < SFX_DEMO_CNT; ++i) {
        if (demo_cfg->func == g_func_cfg_table[i].func)
            func_name = g_func_cfg_table[i].name;
    }
    if (!func_name)
        return;
    sfx_log_show("%-32s: %s\n", "function", func_name);
    sfx_log_show("%-32s: %s\n", "sfx dev", demo_cfg->dev_name);
    sfx_log_show("%-32s: %u\n", "loop run number", demo_cfg->loop_cnt);
    sfx_log_show("%-32s: %u\n", "delay open time(ms)", demo_cfg->per_delay_open);
    sfx_log_show("%-32s: %u\n", "run gap time(ms)", demo_cfg->per_delay_do);
    sfx_log_show("%-32s: %s\n", "param adj switch", demo_cfg->param_adj_func ? "On" : "Off");
    sfx_log_show("%-32s: %s\n", "input file", demo_cfg->in_filename);
    sfx_log_show("%-32s: %s\n", "output file", demo_cfg->out_filename);
    sfx_log_show("%-32s: %u\n", "card num", demo_cfg->card);
    sfx_log_show("%-32s: %u\n", "card device num", demo_cfg->device);
    sfx_log_show("%-32s: %u\n", "sample rate", demo_cfg->rate);
    sfx_log_show("%-32s: %u\n", "sample bits", demo_cfg->bits);
    sfx_log_show("%-32s: %u\n", "channels", demo_cfg->channels);
    sfx_log_show("%-32s: %u\n", "period size", demo_cfg->period_size);
    sfx_log_show("%-32s: %u\n", "period count", demo_cfg->period_count);
    sfx_log_show("%-32s: %u\n", "capture time", demo_cfg->cap_time);
}

int demo_func_data(struct sfx_demo_config *demo_cfg)
{
    int ret;
    FILE *in_file = NULL, *out_file = NULL;
    struct pcm_config pcm_config;
    uint32_t buffer_sz = 0, read_sz = 0, data_sz = 0;
    int read_num;

    struct sfx_ap_dev *spk_ap_dev = NULL;
    struct sfx_pcm_config sfx_pcm_config;
    char *buffer = NULL;
    void *out_data;
    uint32_t out_size;

    if (!demo_cfg) {
        sfx_log_err("demo_cfg invalid\n");
        return -1;
    }
    if (!demo_cfg->in_filename || !demo_cfg->out_filename) {
        sfx_log_err("input wav file or output pcm file invalid\n");
        return -1;
    }

    /* wav parser */
    in_file = fopen(demo_cfg->in_filename, "rb");
    if (!in_file) {
        sfx_log_err("Unable to open file(%s)\n", demo_cfg->in_filename);
        goto err;
    }
    ret = parser_wavhead_to_pcmconfig(in_file, &data_sz, &pcm_config);
    if (ret) {
        sfx_log_err("parser_wavhead_to_pcmconfig failed\n");
        goto err;
    }
    pcm_config.period_size          = demo_cfg->period_size;
    pcm_config.period_count         = demo_cfg->period_count;
    pcm_config.start_threshold      = 0;
    pcm_config.stop_threshold       = 0;
    pcm_config.silence_threshold    = 0;
    out_file = fopen(demo_cfg->out_filename, "wb");
    if (!out_file)    {
        sfx_log_err("unable to create file(%s)\n", demo_cfg->out_filename);
        goto err;
    }
    fseek(out_file, 0, SEEK_SET);

    /* sfx dev open */
    sfx_pcm_config.channels = pcm_config.channels;
    sfx_pcm_config.rate = pcm_config.rate;
    switch (pcm_config.format) {
    case PCM_FORMAT_S16_LE:
        sfx_pcm_config.format = SFX_PCM_FORMAT_S16_LE;
        break;
    case PCM_FORMAT_S24_3LE:
        sfx_pcm_config.format = SFX_PCM_FORMAT_S24_3LE;
        break;
    case PCM_FORMAT_S32_LE:
        sfx_pcm_config.format = SFX_PCM_FORMAT_S32_LE;
        break;
    default:
        sfx_log_err("unsupport pcm format\n");
        break;
    }
    sfx_pcm_config.period_size = pcm_config.period_size;
    sfx_pcm_config.period_count = pcm_config.period_count;
    spk_ap_dev = sfxmgmt_ap_open(g_platform, demo_cfg->dev_name, &sfx_pcm_config);
    if (!spk_ap_dev) {
        sfx_log_err("sfxmgmt_ap_open failed\n");
        goto err;
    }

    /* sfx dev process */
    buffer_sz = sfx_pcm_config.period_size * sfx_pcm_config.channels * (sfx_pcm_config.bit >> 3);
    buffer = malloc(buffer_sz);
    if (!buffer) {
        sfx_log_err("Unable to allocate %d bytes\n", buffer_sz);
        goto err;
    }
    signal(SIGINT, stream_close);
    do {
        read_sz = buffer_sz < data_sz ? buffer_sz : data_sz;
        read_num = fread(buffer, 1, read_sz, in_file);
        if (read_num > 0) {
            ret = sfxmgmt_ap_do(spk_ap_dev, buffer, read_num, &out_data, &out_size);
            if (ret) {
                sfx_log_err("sfx_ap_do_plugin failed\n");
                break;
            }

            if (demo_cfg->per_delay_do)
                usleep(demo_cfg->per_delay_do * 1000);

            /* save file for sfx dev effect */
            fwrite(out_data, 1, out_size, out_file);
            data_sz -= read_num;
        }
    } while (!g_close && read_num > 0 && data_sz > 0);

err:
    if (spk_ap_dev)
        sfxmgmt_ap_close(spk_ap_dev);    /* sfx dev close */
    if (buffer)
        free(buffer);
    if (in_file)
        fclose(in_file);
    if (out_file)
        fclose(out_file);
    return 0;
}

int demo_func_play(struct sfx_demo_config *demo_cfg)
{
    int ret;
    FILE *in_file = NULL, *out_file = NULL;
    bool dump_out = false;
    struct pcm *pcm = NULL;
    struct pcm_config pcm_config;
    uint32_t buffer_sz = 0, read_sz = 0, data_sz = 0;
    int read_num;

    struct sfx_ap_dev *spk_ap_dev = NULL;
    struct sfx_pcm_config sfx_pcm_config;
    char *buffer = NULL;
    void *out_data;
    uint32_t out_size;

    if (!demo_cfg) {
        sfx_log_err("demo_cfg invalid\n");
        return -1;
    }
    if (!demo_cfg->in_filename) {
        sfx_log_err("input wav file invalid\n");
        return -1;
    }

    /* wav parser */
    in_file = fopen(demo_cfg->in_filename, "rb");
    if (!in_file) {
        sfx_log_err("Unable to open file(%s)\n", demo_cfg->in_filename);
        goto err;
    }
    ret = parser_wavhead_to_pcmconfig(in_file, &data_sz, &pcm_config);
    if (ret) {
        sfx_log_err("parser_wavhead_to_pcmconfig failed\n");
        goto err;
    }
    pcm_config.period_size          = demo_cfg->period_size;
    pcm_config.period_count         = demo_cfg->period_count;
    pcm_config.start_threshold      = 0;
    pcm_config.stop_threshold       = 0;
    pcm_config.silence_threshold    = 0;
    if (demo_cfg->out_filename) {
        out_file = fopen(demo_cfg->out_filename, "wb");
        if (!out_file)    {
            sfx_log_err("unable to create file(%s)\n", demo_cfg->out_filename);
            goto err;
        }
        fseek(out_file, 0, SEEK_SET);

        dump_out = true;
    }

    /* sound card */
    pcm = sfx_pcm_open(demo_cfg->card, demo_cfg->device, PCM_OUT, &pcm_config);
    if (!pcm) {
        sfx_log_err("Unsupport params\n");
        goto err;
    } else if (!pcm_is_ready(pcm)) {
        sfx_log_err("Unable to open PCM device %u (%s)\n",
                demo_cfg->device, pcm_get_error(pcm));
        goto err;
    }

    /* sfx dev open */
    sfx_pcm_config.channels = pcm_config.channels;
    sfx_pcm_config.rate = pcm_config.rate;
    switch (pcm_config.format) {
    case PCM_FORMAT_S16_LE:
        sfx_pcm_config.format = SFX_PCM_FORMAT_S16_LE;
        break;
    case PCM_FORMAT_S24_3LE:
        sfx_pcm_config.format = SFX_PCM_FORMAT_S24_3LE;
        break;
    case PCM_FORMAT_S32_LE:
        sfx_pcm_config.format = SFX_PCM_FORMAT_S32_LE;
        break;
    default:
        sfx_log_err("unsupport pcm format\n");
        break;
    }
    sfx_pcm_config.period_size = pcm_config.period_size;
    sfx_pcm_config.period_count = pcm_config.period_count;
    spk_ap_dev = sfxmgmt_ap_open(g_platform, demo_cfg->dev_name, &sfx_pcm_config);
    if (!spk_ap_dev) {
        sfx_log_err("sfxmgmt_ap_open failed\n");
        goto err;
    }

    /* sfx dev process */
    buffer_sz = pcm_frames_to_bytes(pcm, sfx_pcm_config.period_size);
    buffer = malloc(buffer_sz);
    if (!buffer) {
        sfx_log_err("Unable to allocate %d bytes\n", buffer_sz);
        goto err;
    }
    signal(SIGINT, stream_close);
    do {
        read_sz = buffer_sz < data_sz ? buffer_sz : data_sz;
        read_num = fread(buffer, 1, read_sz, in_file);
        if (read_num > 0) {
            ret = sfxmgmt_ap_do(spk_ap_dev, buffer, read_num, &out_data, &out_size);
            if (ret) {
                sfx_log_err("sfx_ap_do_plugin failed\n");
                break;
            }

            /* save file for sfx dev effect */
            if (dump_out)
                fwrite(out_data, 1, out_size, out_file);

            /* for tinyalsa 1.1.1, the return value of pcm_write
             * may positive number when successfully executed
             */
            if (pcm_write(pcm, out_data, out_size) < 0) {
                sfx_log_err("Error playing sample\n");
                break;
            }
            data_sz -= read_num;
        }
    } while (!g_close && read_num > 0 && data_sz > 0);

err:
    if (spk_ap_dev)
        sfxmgmt_ap_close(spk_ap_dev);    /* sfx dev close */
    if (buffer)
        free(buffer);
    if (pcm)
        pcm_close(pcm);
    if (in_file)
        fclose(in_file);
    if (out_file)
        fclose(out_file);
    return 0;
}

int demo_func_cap(struct sfx_demo_config *demo_cfg)
{
    int ret;
    FILE *out_raw_file = NULL, *out_file = NULL;
    char *out_raw_filename = NULL;
    struct pcm *pcm = NULL;
    struct pcm_config pcm_config;
    uint32_t buffer_sz = 0, write_sz = 0, write_sz_p = 0;
    unsigned int bytes_read = 0;
    unsigned int frames;
    struct timespec end;
    struct timespec now;

    struct sfx_ap_dev *cap_ap_dev = NULL;
    struct sfx_pcm_config sfx_pcm_config;
    char *buffer = NULL;
    void *out_data;
    uint32_t out_size;

    if (!demo_cfg) {
        sfx_log_err("demo_cfg invalid\n");
        return -1;
    }
    if (!demo_cfg->out_filename) {
        sfx_log_err("out_filename invalid\n");
        return -1;
    }

    memset(&pcm_config, 0, sizeof(pcm_config));
    pcm_config.period_size          = demo_cfg->period_size;
    pcm_config.period_count         = demo_cfg->period_count;
    pcm_config.channels             = demo_cfg->channels;
    pcm_config.rate                 = demo_cfg->rate;
    pcm_config.start_threshold      = 0;
    pcm_config.stop_threshold       = 0;
    pcm_config.silence_threshold    = 0;

    switch (demo_cfg->bits) {
    case 32:
        pcm_config.format = PCM_FORMAT_S32_LE;
        break;
    case 24:
        pcm_config.format = PCM_FORMAT_S24_LE;
        break;
    case 16:
        pcm_config.format = PCM_FORMAT_S16_LE;
        break;
    default:
        fprintf(stderr, "%u bits is not supported.\n", demo_cfg->bits);
        return -1;
    }

    out_raw_filename = calloc(1, strlen(demo_cfg->out_filename) + 5);
    if (!out_raw_filename) {
        sfx_log_err("no memory\n");
        goto err;
    }
    if (snprintf(out_raw_filename, (strlen(demo_cfg->out_filename) + 5),
        "%s-raw", demo_cfg->out_filename) < 0) {
        sfx_log_err("snprintf failed\n");
        goto err;
    }

    out_file = fopen(demo_cfg->out_filename, "wb");
    if (!out_file)    {
        sfx_log_err("unable to create file(%s)\n", demo_cfg->out_filename);
        goto err;
    }
    fseek(out_file, 0, SEEK_SET);

    out_raw_file = fopen(out_raw_filename, "wb");
    if (!out_raw_file)    {
        sfx_log_err("unable to create file(%s)\n", out_raw_filename);
        goto err;
    }

    /* sound card */
    pcm = sfx_pcm_open(demo_cfg->card, demo_cfg->device, PCM_IN, &pcm_config);
    if (!pcm || !pcm_is_ready(pcm)) {
        sfx_log_err("Unable to open PCM device %u (%s)\n",
                demo_cfg->device, pcm_get_error(pcm));
        goto err;
    }

    /* sfx dev open */
    sfx_pcm_config.channels = pcm_config.channels;
    sfx_pcm_config.rate = pcm_config.rate;
    switch (pcm_config.format) {
    case PCM_FORMAT_S16_LE:
        sfx_pcm_config.format = SFX_PCM_FORMAT_S16_LE;
        break;
    case PCM_FORMAT_S24_3LE:
        sfx_pcm_config.format = SFX_PCM_FORMAT_S24_3LE;
        break;
    case PCM_FORMAT_S32_LE:
        sfx_pcm_config.format = SFX_PCM_FORMAT_S32_LE;
        break;
    default:
        sfx_log_err("unsupport pcm format\n");
        break;
    }
    sfx_pcm_config.period_size = pcm_config.period_size;
    sfx_pcm_config.period_count = pcm_config.period_count;

    cap_ap_dev = sfxmgmt_ap_open(g_platform, demo_cfg->dev_name, &sfx_pcm_config);
    if (!cap_ap_dev) {
        sfx_log_err("sfxmgmt_ap_open failed\n");
        goto err;
    }

    /* sfx dev process */
    buffer_sz = pcm_frames_to_bytes(pcm, sfx_pcm_config.period_size);
    buffer = malloc(buffer_sz);
    if (!buffer) {
        sfx_log_err("Unable to allocate %d bytes\n", buffer_sz);
        goto err;
    }

    clock_gettime(CLOCK_MONOTONIC, &now);
    end.tv_sec = now.tv_sec + demo_cfg->cap_time;
    end.tv_nsec = now.tv_nsec;

    signal(SIGINT, stream_close);
    while (!g_close && !pcm_read(pcm, buffer, buffer_sz)) {
        write_sz = fwrite(buffer, 1, buffer_sz, out_raw_file);
        if (write_sz != buffer_sz) {
            sfx_log_err("want_write:%d, write_size:%d\n", buffer_sz, write_sz);
            fprintf(stderr,"Error capturing sample\n");
            break;
        }

        ret = sfxmgmt_ap_do(cap_ap_dev, buffer, buffer_sz, &out_data, &out_size);
        if (ret) {
            sfx_log_err("sfx_ap_do_plugin failed\n");
            break;
        }
        write_sz_p = fwrite(out_data, 1, out_size, out_file);
        if (write_sz_p != out_size) {
            sfx_log_err("want_write:%d, write_size:%d\n", out_size, write_sz_p);
            fprintf(stderr,"Error capturing sample\n");
            break;
        }

        bytes_read += out_size;
        if (demo_cfg->cap_time) {
            clock_gettime(CLOCK_MONOTONIC, &now);
            if (now.tv_sec > end.tv_sec ||
                (now.tv_sec == end.tv_sec && now.tv_nsec >= end.tv_nsec))
                break;
        }
    }

    frames = pcm_bytes_to_frames(pcm, bytes_read);
    sfx_log_show("Captured %u frames\n", frames);

err:
    if (cap_ap_dev)
        sfxmgmt_ap_close(cap_ap_dev);    /* sfx dev close */
    if (buffer)
        free(buffer);

    if (pcm)
        pcm_close(pcm);
    if (out_raw_file)
        fclose(out_raw_file);
    if (out_file)
        fclose(out_file);
    if (out_raw_filename)
        free(out_raw_filename);
    return 0;
}

int demo_func_version(struct sfx_demo_config *demo_cfg)
{
    (void)demo_cfg;

    sfxmgmt_show_version(g_platform);

    return 0;
}
