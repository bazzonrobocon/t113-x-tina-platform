// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * sfx_parser.c -- sfx for sw effect parser sfx
 *
 * Dby <dby@allwinnertech.com>
 *
 * Copyright (c) 2023 Allwinnertech Ltd.
 */

#include <stdlib.h>
#include <string.h>

#include "sfx_log.h"
#include "sfx.h"
#include "sfx_parser.h"

void sfx_param_show(struct sfx_params *params, unsigned int params_cnt)
{
    struct sfx_param *param;
    uint8_t i;

    sfx_log_debug("\n");

    if (!params || params_cnt == 0) {
        sfx_log_err("params info is null, or params cnt is 0\n");
        return;
    }

    /*******************************************************************************************
     * Display the parsed parameters, the following are just examples.
     ******************************************************************************************/

    for (i = 0; i < params_cnt; i++) {
        param = &params[i].param;
        sfx_log_debug("path(devid)[%u], param_id [%u], dev_enable [%u]\n",
                      params[i].id.dev_id, params[i].param_id, params[i].dev_enable);

        sfx_log_debug("tmp1: %u, tmp2: %u\n", param->tmp1, param->tmp2);
    }
}

int sfx_param_parser(struct sfx_params **params, uint32_t *params_cnt, char *params_file)
{
    struct sfx_params *params_delta = NULL;
    unsigned int params_cnt_delta = 0;

    sfx_log_debug("\n");

    if (!params_file) {
        sfx_log_err("params file is invalid\n");
        return -1;
    }

    /*******************************************************************************************
     * According to the parameters required by the algorithm and
     * the parsing method (such as xml), it can be realized on demand.
     ******************************************************************************************/

    *params = params_delta;
    *params_cnt = params_cnt_delta;
    return 0;
}

int sfx_param_free(struct sfx_params *params, uint32_t params_cnt)
{
    sfx_log_debug("\n");

    if (!params || params_cnt == 0) {
        sfx_log_err("params info is null, or params cnt is 0\n");
        return -1;
    }

    /*******************************************************************************************
     * Release the resources requested by sfx_param_parser().
     ******************************************************************************************/

    /* params = NULL;
     * params_cnt = 0;
     */

    return 0;
}
