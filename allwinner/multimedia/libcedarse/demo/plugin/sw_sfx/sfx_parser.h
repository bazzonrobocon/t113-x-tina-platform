/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * sfx_parser.h -- sfx for sw effect parser sfx api
 *
 * Dby <dby@allwinnertech.com>
 *
 * Copyright (c) 2023 Allwinnertech Ltd.
 */

#ifndef __SFX_PARSER_H
#define __SFX_PARSER_H

#include <stdint.h>
#include <stdbool.h>

#include "sfx_list.h"
#include "sfx_mgmt.h"

#ifdef CONFIG_SFX_TARGET_ANDROID
#define SFX_MGMT_AUDIO_SFX_PARAM        NULL
#elif CONFIG_SFX_TARGET_BUILDROOT
#define SFX_MGMT_AUDIO_SFX_PARAM        NULL
#else
#define SFX_MGMT_AUDIO_SFX_PARAM        NULL
#endif

/* It is used to parse algorithm parameters and is not required to be implemented.
 * The following are just parsing examples.
 */

struct sfx_params {
    struct sfx_list_head list;          /* param list */
    struct sfx_param_id id;             /* which audio dev is this param */

    uint8_t param_id;                   /* the sequenece number in the param pool */
    bool dev_enable;                    /* whether algorithm is enable in the audio dev */

    struct sfx_pcm_config pcm_config;   /* pcm info */

    struct sfx_param param;             /* sfx param */
};

void sfx_param_show(struct sfx_params *params, uint32_t params_cnt);
int sfx_param_parser(struct sfx_params **params, uint32_t *params_cnt, char *params_file);
int sfx_param_free(struct sfx_params *params, uint32_t params_cnt);

#endif /* __SFX_PARSER_H */