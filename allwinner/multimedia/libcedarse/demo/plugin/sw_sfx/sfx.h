/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * sfx.h -- sfx api
 *
 * Dby <dby@allwinnertech.com>
 *
 * Copyright (c) 2023 Allwinnertech Ltd.
 */

#ifndef __SFX_H
#define __SFX_H

#define SFX_VERSION        "1.0.0"

struct sfx_param {
    unsigned int tmp1;
    unsigned int tmp2;
};

int sfx_init(struct sfx_param *param);
void sfx_exit(struct sfx_param *param);
int sfx_process(struct sfx_param *param,
                void *in_data, unsigned int in_size,
                void **out_data, unsigned int *out_size);

#endif /* __SFX_H */