/*
 * Copyright (c) 2019-2025 Allwinner Technology Co., Ltd. ALL rights reserved.
 *
 * Allwinner is a trademark of Allwinner Technology Co.,Ltd., registered in
 * the the people's Republic of China and other countries.
 * All Allwinner Technology Co.,Ltd. trademarks are used with permission.
 *
 * sfx_mgmt.h -- sfx for mgmt api
 *
 * Dby <dby@allwinnertech.com>
 */

#ifndef __SFX_MGMT_H
#define __SFX_MGMT_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <stdlib.h>
#include <stdbool.h>

#define SFX_MGMT_AUDIO_DEVICE_DESC sfxmgmt_get_desc_path()

enum sfx_pcm_format {
    SFX_PCM_FORMAT_INVALID = -1,
    SFX_PCM_FORMAT_S16_LE = 0,  /* 16-bit signed */
    SFX_PCM_FORMAT_S32_LE,      /* 32-bit signed */
    SFX_PCM_FORMAT_S8,          /* 8-bit signed */
    SFX_PCM_FORMAT_S24_LE,      /* 24-bits in 4-bytes */
    SFX_PCM_FORMAT_S24_3LE,     /* 24-bits in 3-bytes */

    SFX_PCM_FORMAT_MAX,
};

enum sfx_pcm_data_mode {
    SFX_PCM_DATA_I = 0,
    SFX_PCM_DATA_N,
};

typedef struct sfx_pcm_config {
    uint32_t channels;
    uint32_t rate;
    uint32_t bit;
    enum sfx_pcm_format format;
    uint32_t period_size;
    uint32_t period_count;

    enum sfx_pcm_data_mode data_mode;
} sfx_pcm_cfg_t;

enum SFX_STREAM {
    SFX_STREAM_PLAYBACK = 0,
    SFX_STREAM_CAPTURE,
};

struct sfx_effect_unit {
    char *name;
    char *param_path;
};

struct sfx_effect_table {
    const char *ap_name;
    uint8_t ap_sub_id;

    struct sfx_plugin *ap;

    /* debug: dump_pcm_cnt -> dump the number of loop files processed by this ap */
    unsigned int dump_pcm_cnt;
};

struct sfx_dev_runtime {
    int running;

    struct sfx_pcm_config inpcm_config;
    struct sfx_pcm_config outpcm_config;

    unsigned int in_frames;
    unsigned int in_ch_size;
    unsigned int inpcm_size;
    char *inpcm_plane;
    char *inpcm_plane_table;

    unsigned int out_frames;
    unsigned int out_ch_size;
    unsigned int outpcm_size;
    char *outpcm_interleaved;
};

struct sfx_audio_device {
    int dev_id;
    char dev_name[32];
    enum SFX_STREAM stream;
    struct sfx_pcm_config pcm_config;

    uint8_t hw_effect_cnt;
    struct sfx_effect_table *hw_effect_table;
    uint8_t sw_effect_cnt;
    struct sfx_effect_table *sw_effect_table;
    uint8_t rp_effect_cnt;
    struct sfx_effect_table *rp_effect_table;
    unsigned int effect_cnt_all;

    void *priv;     /* point sfx_platform */
    struct rpsfx_io *rpsfx_io;

    /* runtime */
    struct sfx_dev_runtime runtime;
    pthread_mutex_t oc_lock;

    /* debug */
    bool dump_time;
    char *dump_time_name;
    FILE *dump_time_file;
    long long satrt_us;
};

/* Misc api: get all register sound device */
struct sfx_devid {
    int dev_id;
    char *dev_name;
};

struct sfx_reg_dev {
    struct sfx_devid *devid;
    uint8_t devid_cnt;
};

int sfxmgmt_reg_dev_get(struct sfx_reg_dev **reg_dev);

/* Misc api: ap <-> param match check */
struct sfx_param_id {
    int dev_id;        /* match sfx_plugin->dev_id */
    uint8_t ap_sub_id;    /* match sfx_plugin->ap_sub_id */
};

int sfxmgmt_param_match(struct sfx_param_id *id1, struct sfx_param_id *id2);
int sfxmgmt_param_match_ap(struct sfx_plugin *ap, struct sfx_param_id *param_id);

/* Basic api handle */
struct sfx_platform {
    /* platform name */
    char name[32];
    char board[32];

    /* hwio memory */
    unsigned int hwsfx_base;
    unsigned int hwsfx_size;

    /* sound effect unit info */
    uint8_t effect_unit_cnt;
    struct sfx_effect_unit *effect_unit;

    /* register audio device */
    bool hw_effect_exist;
    bool rp_effect_exist;
    uint8_t audio_dev_cnt;
    struct sfx_list_head *ap_list;
    struct sfx_audio_device *audio_dev;

    /* register sound effect */
    uint8_t reg_ap_cnt;
    struct sfx_plugin *reg_ap;

    /* debug info */
    char *dump_dirpath;

    /* public resource */
    pthread_rwlock_t param_setup_lock;
};

struct sfx_ap_dev {
    struct sfx_list_head *ap_list;
    struct sfx_audio_device *audio_dev;
};

/* Basic api: pcm stream */
int sfxmgmt_init(struct sfx_platform **platform_info, char *desc_file);
void sfxmgmt_exit(struct sfx_platform *platform_info);

struct sfx_ap_dev *sfxmgmt_ap_open(struct sfx_platform *platform,
                                   const char *dev_name,
                                   struct sfx_pcm_config *pcm_config);
int sfxmgmt_ap_close(struct sfx_ap_dev *ap_dev);
int sfxmgmt_ap_do(struct sfx_ap_dev *ap_dev,
                  void *data, uint32_t size,
                  void **data_out, uint32_t *data_out_size);

/* Basic api: parm setup */
int sfxmgmt_parm_adjust_server(struct sfx_platform *platform);
int sfxmgmt_parm_setup(struct sfx_platform *platform, const char *dev_name,
                       const char *ap_name, unsigned int ap_sub_id, void *param);
int sfxmgmt_parm_setup_done(struct sfx_platform *platform, const char *dev_name);
int sfxmgmt_parm_setup_sync(struct sfx_platform *platform, const char *dev_name,
                            const char *ap_name, unsigned int ap_sub_id, void *param);

/* Basic api: print frame and register sound effect version */
void sfxmgmt_show_version(struct sfx_platform *platform);

char *sfxmgmt_get_desc_path();

/* pcm data convert */
int sfxmgmt_interleaved_to_plane(void *i_data, uint32_t size, void *p_data,
                                 struct sfx_pcm_config pcm_config, unsigned int frames);
int sfxmgmt_plane_to_interleaved(void *p_data, uint32_t size, void *i_data,
                                 struct sfx_pcm_config pcm_config, unsigned int frames);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __SFX_MGMT_H */
