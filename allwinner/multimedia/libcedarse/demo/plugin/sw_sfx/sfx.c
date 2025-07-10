// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * sfx.h -- sfx
 *
 * Dby <dby@allwinnertech.com>
 *
 * Copyright (c) 2023 Allwinnertech Ltd.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "sfx.h"

int sfx_init(struct sfx_param *param)
{
    (void)param;
    printf("[%s->%d]\n", __func__, __LINE__);

    /*******************************************************************************************
     * Algorithm initialization.
     ******************************************************************************************/

    return 0;
}

void sfx_exit(struct sfx_param *param)
{
    (void)param;
    printf("[%s->%d]\n", __func__, __LINE__);

    /*******************************************************************************************
     * Algorithmic destruction.
     ******************************************************************************************/

    return;
}

int sfx_process(struct sfx_param *param,
                void *in_data, uint32_t in_size,
                void **out_data, uint32_t *out_size)
{
    (void)param;
    printf("[%s->%d]\n", __func__, __LINE__);

    /*******************************************************************************************
     * Algorithmic data processing.
     ******************************************************************************************/

    *out_data = in_data;
    *out_size = in_size;

    return 0;
}
