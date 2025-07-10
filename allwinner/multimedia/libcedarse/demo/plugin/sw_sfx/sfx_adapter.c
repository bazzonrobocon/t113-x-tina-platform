// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * sfx_adapter.c -- sfx for sw effect adapter sfx
 *
 * Dby <dby@allwinnertech.com>
 *
 * Copyright (c) 2023 Allwinnertech Ltd.
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "sfx_log.h"
#include "sfx_plugin.h"
#include "sfx.h"
#include "sfx_parser.h"

SFX_LIST_HEAD(sfx_params_list);

int ap_sfx_param_parser_all(struct sfx_list_head *param_list, char *params_file)
{
    struct sfx_params *params = NULL;
    struct sfx_params *params_tmp;
    struct sfx_params *params_add;
    uint32_t i, params_cnt = 0;
    int ret;

    sfx_log_debug("\n");

    if (!param_list || !params_file) {
        sfx_log_err("param_list or params_file invalid\n");
        return -1;
    }

    /*******************************************************************************************
     * Realize according to requirements, The following are just parsing examples.
     ******************************************************************************************/

    ret = sfx_param_parser(&params, &params_cnt, params_file);
    if (ret) {
        sfx_log_err("sfx_param_parser failed\n");
        return -1;
    }
    if (!params || params_cnt == 0) {
        sfx_log_err("params is invalid, or params is 0\n");
        return -1;
    }

    for (i = 0; i < params_cnt; i++) {
        params_tmp = &params[i];
        if (!params_tmp) {
            sfx_log_err("params is invaild\n");
            goto err;
        }

        params_add = calloc(1, sizeof(*params_add));
        if (!params_add) {
            sfx_log_err("no memory\n");
            goto err;
        }
        memcpy(params_add, params_tmp, sizeof(*params_add));
        sfx_list_add_tail(&params_add->list, param_list);
    }

    ret = sfx_param_free(params, params_cnt);
    if (ret) {
        sfx_log_err("sfx_param_free failed\n");
        goto err;
    }

    return 0;

err:
    if (params_add)
        free(params_add);
    if (params && params_cnt) {
        ret = sfx_param_free(params, params_cnt);
        if (ret) {
            sfx_log_err("sfx_param_free failed\n");
        }
    }
    return -1;
}

void ap_sfx_param_free_all(struct sfx_list_head *param_list)
{
    struct sfx_params *params_del, *c;

    sfx_log_debug("\n");

    /*******************************************************************************************
     * Realize according to requirements, The following are just parsing examples.
     ******************************************************************************************/

    sfx_list_for_each_entry_safe(params_del, c, param_list, list) {
        sfx_list_del(&params_del->list);
        if (params_del) {
            free(params_del);
            params_del = NULL;
        }
    }
}

static int ap_sfx_init(struct sfx_plugin *ap)
{
    struct sfx_params *params, *c;
    bool params_find = false;
    int ret;

    sfx_log_debug("\n");

    if (!ap) {
        sfx_log_err("sfx_plugin is invaild\n");
        return -1;
    }

    /*******************************************************************************************
     * Realize according to requirements, The following are just parsing examples.
     ******************************************************************************************/

    /* Get the parameters that match the ap */
    sfx_list_for_each_entry_safe(params, c, ap->param_list, list) {
        if (sfxmgmt_param_match_ap(ap, &params->id)) {
            params_find = true;
            break;
        }
    }
    if (!params_find) {
        sfx_log_err("params_find mismatch, ap dev id %u\n", ap->dev_id);
        return -1;
    }
    if (!params) {
        sfx_log_err("params is invaild\n");
        return -1;
    }

    ret = sfx_init(&params->param);
    if (ret) {
        sfx_log_err("sfx init failed\n");
        return -1;
    }

    if (params->dev_enable) {
        ap->ap_updata_mode(ap, AP_MODE_WORK);
    } else {
        ap->ap_updata_mode(ap, AP_MODE_BYPASS);
    }

    ap->priv = params;

    return 0;
}

static int ap_sfx_exit(struct sfx_plugin *ap)
{
    struct sfx_params *params;

    sfx_log_debug("\n");

    if (!ap) {
        sfx_log_err("sfx_plugin is invaild\n");
        return -1;
    }

    /*******************************************************************************************
     * Realize according to requirements, The following are just parsing examples.
     ******************************************************************************************/

    params = (struct sfx_params *)ap->priv;
    if (!params) {
        sfx_log_err("sfx is invaild\n");
        return -1;
    }

    sfx_exit(&params->param);

    return 0;
}

static int ap_sfx_process(struct sfx_plugin *ap,
                          void *in_data, uint32_t in_size,
                          void **out_data, uint32_t *out_size)
{
    struct sfx_params *params;
    int ret;

    /* sfx_log_debug("\n"); */

    if (!ap) {
        sfx_log_err("sfx_plugin is invaild\n");
        return -1;
    }

    /*******************************************************************************************
     * Realize according to requirements, The following are just parsing examples.
     ******************************************************************************************/

    params = (struct sfx_params *)ap->priv;
    if (!params) {
        sfx_log_err("params is invaild\n");
        return -1;
    }

    ret = sfx_process(&params->param, in_data, in_size, out_data, out_size);
    if (ret) {
        sfx_log_err("sfx_process failed\n");
        return -1;
    }

    return 0;
}

static int ap_sfx_setup(struct sfx_plugin *ap, void *param)
{
    struct sfx_params *params_tmp = NULL;
    struct sfx_params *params_add = NULL;
    struct sfx_params *params_del = NULL, *c;
    bool params_find = false;

    sfx_log_debug("\n");

    if (!ap) {
        sfx_log_err("sfx_plugin is invaild\n");
        return -1;
    }
    if (!param) {
        sfx_log_err("param is invaild\n");
        return -1;
    }

    /*******************************************************************************************
     * Realize according to requirements, The following are just parsing examples.
     ******************************************************************************************/

    params_tmp = (struct sfx_params *)param;
    if (!params_tmp) {
        sfx_log_err("params is invaild\n");
        return -1;
    }
    if (ap->dev_id != params_tmp->id.dev_id) {
        sfx_log_err("ap dev_id and param dev_id mismatch\n");
        return -1;
    }

    /* del old param form params list */
    sfx_list_for_each_entry_safe(params_del, c, ap->param_list, list) {
        if (sfxmgmt_param_match_ap(ap, &params_del->id)) {
            params_find = true;
            break;
        }
    }
    if (!params_find) {
        sfx_log_err("params mismatch, ap dev id %u\n", ap->dev_id);
        return -1;
    }
    if (!params_del) {
        sfx_log_err("params is invaild\n");
        return -1;
    }
    sfx_log_debug("params_del %p\n", params_del);
    sfx_list_del(&params_del->list);
    free(params_del);
    params_del = NULL;

    /* add new param to params list */
    params_add = calloc(1, sizeof(*params_add));
    if (!params_add) {
        sfx_log_err("no memory\n");
        return -1;
    }
    sfx_log_debug("params_add %p\n", params_add);
    memcpy(params_add, params_tmp, sizeof(*params_add));
    sfx_list_add_tail(&params_add->list, ap->param_list);

    /* updata param to ap priv */
    ap->priv = params_add;

    /* updata mode */
    if (params_add->dev_enable) {
        ap->ap_updata_mode(ap, AP_MODE_WORK);
    } else {
        ap->ap_updata_mode(ap, AP_MODE_BYPASS);
    }

    sfx_log_debug("param setup success\n");

    return 0;
}

int ap_sfx_updata_mode(struct sfx_plugin *ap, enum AP_MODE mode)
{
    sfx_log_debug("\n");

    if (!ap) {
        sfx_log_err("sfx_plugin is invaild\n");
        return -1;
    }

    ap->mode = mode;
    ap->proc_sign = AP_PROC_CYCLE;
    sfx_log_debug("updata mode to %s\n", mode ? "WORK" : "BYPASS");

    return 0;
}

int ap_sfx_param_parser_unit(void **param, char *params_file, struct sfx_param_id *param_id)
{
    struct sfx_params *params = NULL;
    struct sfx_params *params_tmp = NULL;
    struct sfx_params *params_add = NULL;
    bool params_find = false;
    uint32_t i, params_cnt = 0;

    sfx_log_debug("params file %s\n", params_file);

    if (!params_file) {
        sfx_log_err("params_file is invalid\n");
        return -1;
    }

    /*******************************************************************************************
     * Realize according to requirements, The following are just parsing examples.
     ******************************************************************************************/

    if (sfx_param_parser(&params, &params_cnt, params_file)) {
        sfx_log_err("sfx_param_parser failed\n");
        return -1;
    }
    if (!params || params_cnt == 0) {
        sfx_log_err("params is invalid, or params_cnt is 0\n");
        return -1;
    }
    for (i = 0; i < params_cnt; i++) {
        if (sfxmgmt_param_match(&params[i].id, param_id)) {
            params_tmp = &params[i];
            params_find = true;
            break;
        }
    }
    if (!params_find) {
        sfx_log_err("params file(%s) has no required devid(%d) ap sub id(%u) param\n",
                    params_file, param_id->dev_id, param_id->ap_sub_id);
        goto err;
    }
    if (!params_tmp) {
        sfx_log_err("params is invaild\n");
        goto err;
    }

    params_add = calloc(1, sizeof(*params_add));
    if (!params_add) {
        sfx_log_err("no memory\n");
        goto err;
    }
    memcpy(params_add, params_tmp, sizeof(*params_add));

    if (sfx_param_free(params, params_cnt)) {
        sfx_log_err("sfx_param_free failed\n");
        goto err;
    }

    *param = params_add;
    return 0;

err:
    if (params_add)
        free(params_add);
    if (params && params_cnt) {
        if (sfx_param_free(params, params_cnt))
            sfx_log_err("sfx_param_free failed\n");
    }
    *param = NULL;
    return -1;
}

void ap_sfx_param_free_uint(void *param)
{
    struct sfx_params *params_del = NULL;

    sfx_log_debug("\n");

    if (!param) {
        sfx_log_err("param is invalid\n");
        return;
    }

    /*******************************************************************************************
     * Realize according to requirements, The following are just parsing examples.
     ******************************************************************************************/

    params_del = param;
    if (params_del) {
        free(params_del);
        params_del = NULL;
    }
}

static char *ap_sfx_version(void)
{
    return SFX_VERSION;
}

struct sfx_plugin ap_sfx = {
    .ap_name                = "SW_SFX_DEMO",
    .ap_init                = ap_sfx_init,
    .ap_exit                = ap_sfx_exit,
    .ap_process             = ap_sfx_process,

    .ap_setup               = ap_sfx_setup,             /* not necessary */
    .ap_updata_mode         = ap_sfx_updata_mode,       /* not necessary */
    .ap_param_parser_all    = ap_sfx_param_parser_all,  /* not necessary */
    .ap_param_free_all      = ap_sfx_param_free_all,    /* not necessary */
    .ap_param_parser_unit   = ap_sfx_param_parser_unit, /* not necessary */
    .ap_param_free_unit     = ap_sfx_param_free_uint,   /* not necessary */
    .ap_get_version         = ap_sfx_version,           /* not necessary */

    .type                   = AP_TYPE_SW,
    .domain                 = AP_DOMAIN_SYS,
    .param_list             = &sfx_params_list,
    .param_file             = SFX_MGMT_AUDIO_SFX_PARAM,
};
REGISTER_SFX_MGMT_AP(ap_sfx);
