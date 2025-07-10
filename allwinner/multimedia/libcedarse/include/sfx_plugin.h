/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * sfx_plugin.h -- sfx for audio plugin api
 *
 * Dby <dby@allwinnertech.com>
 *
 * Copyright (c) 2023 Allwinnertech Ltd.
 */

#ifndef __SFX_PLUGIN_H
#define __SFX_PLUGIN_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <stdio.h>
#include <stdint.h>
#include "sfx_list.h"
#include "sfx_mgmt.h"

#define SECTION(x)  __attribute__((used,section(x)))

#define REGISTER_SFX_MGMT_AP(ap) \
    const sfx_plugin_t sfx_mgmt_ap_##ap SECTION("sfx_mgmt_ap") = &(ap)

#define sfx_free(__ptr) {   \
        if (__ptr) {        \
            free(__ptr);    \
            __ptr = NULL;   \
        }                   \
    }

enum AP_MODE {
    AP_MODE_BYPASS = 0,
    AP_MODE_WORK,
};

enum AP_TYPE {
    AP_TYPE_HW = 0,
    AP_TYPE_SW,
    AP_TYPE_RP,
};

enum AP_DOMAIN {
    AP_DOMAIN_SYS = 0,
    AP_DOMAIN_DSP,
    AP_DOMAIN_RV,
    AP_DOMAIN_CNT,
};

enum AP_STREAM {
    AP_STREAM_PLAYBACK = 0,
    AP_STREAM_CAPTURE,
};

enum AP_PROCESS_SIGN {
    AP_PROC_CYCLE = 0,
    AP_PROC_ONCE,
    AP_PROC_COMPLETE,
};

struct sfx_plugin {
    struct sfx_list_head list;
    const char *ap_name;
    uint8_t ap_sub_id;

    int (*ap_init)(struct sfx_plugin *ap);
    int (*ap_exit)(struct sfx_plugin *ap);
    int (*ap_process)(struct sfx_plugin *ap,
                      void *in_data, uint32_t in_size,
                      void **out_data, uint32_t *out_size);
    int (*ap_setup)(struct sfx_plugin *ap, void *param);
    int (*ap_updata_mode)(struct sfx_plugin *ap, enum AP_MODE mode);

    int (*ap_param_parser_all)(struct sfx_list_head *param_list, char *params_file);
    void (*ap_param_free_all)(struct sfx_list_head *param_list);
    int (*ap_param_parser_unit)(void **param, char *params_file, struct sfx_param_id *param_id);
    void (*ap_param_free_unit)(void *param);

    char *(*ap_get_version)(void);

    void *priv;                         /* point sound effect param */

    /* attribute config */
    const enum AP_TYPE type;
    const enum AP_DOMAIN domain;
    enum AP_STREAM stream;
    int dev_id;
    const char *param_file;             /* will change if custom in config file */
    struct sfx_list_head *param_list;

    /* remoute proccess io */
    struct rpsfx_io *rpsfx_io;
    uint8_t rpsfx_pool_id;

    /* runtime config */
    enum AP_MODE mode;
    enum AP_PROCESS_SIGN proc_sign;
    struct sfx_pcm_config inpcm_config;
    struct sfx_pcm_config outpcm_config;
    struct sfx_dev_runtime *dev_runtime;
    unsigned int frames;

    /* debug info */
    unsigned int dump_last_num;
    unsigned int dump_data_cnt;
    char *dump_file_name;
    FILE *dump_file;
    uint32_t dump_buf_len;
    int8_t *dump_buf;
};

typedef struct sfx_plugin* sfx_plugin_t;

int ap_add_plugin(struct sfx_list_head *head, struct sfx_plugin *ap, int8_t ap_num);
int ap_del_plugin(struct sfx_list_head *head, struct sfx_plugin *ap);
int ap_del_plugin_all(struct sfx_list_head *head);
int ap_init_plugin_all(struct sfx_list_head *head);
int ap_exit_plugin_all(struct sfx_list_head *head);
int ap_do_plugin(struct sfx_list_head *head,
                 struct sfx_audio_device *audio_dev,
                 void *data, uint32_t size,
                 void **data_out, uint32_t *data_out_size);

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* __SFX_PLUGIN_H */
