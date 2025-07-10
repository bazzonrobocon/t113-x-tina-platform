/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
 * sfx_log.h -- sfx for log api
 *
 * Dby <dby@allwinnertech.com>
 *
 * Copyright (c) 2023 Allwinnertech Ltd.
 */

#ifndef __SFX_LOG_H
#define __SFX_LOG_H

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#if defined(CONFIG_ARCH_ARM)
#define AP_CORE "ARM-"
#elif defined(CONFIG_ARCH_RISCV)
#define AP_CORE "RV-"
#elif defined(CONFIG_ARCH_DSP)
#define AP_CORE "DSP-"
#else
#define AP_CORE ""
#endif

/* menuconfig->Platform Selection->Print set->Open debug print */
#ifdef CONFIG_SFXMGMT_PRINT_DEBUG_ON
#define SFX_LOG_DEBUG    1
#else
#define SFX_LOG_DEBUG    0
#endif

/* menuconfig->Platform Selection->Print set->Unify print to terminal */
#ifdef CONFIG_SFXMGMT_PRINT_TO_TERMINAL
/* unify print to terminal. */
#if SFX_LOG_DEBUG
#define sfx_log_debug(fmt, args...) \
    printf("[%s-DBG][%s %d] " fmt, AP_CORE, __func__, __LINE__, ##args)
#else
#define sfx_log_debug(fmt, args...) \
    do {                            \
        if (false)                  \
            printf(fmt, ##args);    \
    } while (false)
#endif

#define sfx_log_info(fmt, args...)  \
    printf("[%s-INF][%s %d] " fmt, AP_CORE, __func__, __LINE__, ##args)

#define sfx_log_warn(fmt, args...)  \
    printf("[%s-WARN][%s %d] " fmt, AP_CORE, __func__, __LINE__, ##args)

#define sfx_log_err(fmt, args...)   \
    printf("[%s-ERR][%s %d] " fmt, AP_CORE, __func__, __LINE__, ##args)

#define sfx_log_show(fmt, args...)  \
    printf(fmt, ##args)

#else
/* print to logcat when target is android and not defined SFXMGMT_PRINT_TO_TERMINAL. */
#ifdef CONFIG_SFX_TARGET_ANDROID
#include <android/log.h>

#ifndef LOG_TAG
#define LOG_TAG "SFX"
#endif

#if SFX_LOG_DEBUG
#define sfx_log_debug(fmt, args...) \
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "[%s-DBG][%s %d] " fmt, AP_CORE, __func__, __LINE__, ##args);
#else
#define sfx_log_debug(fmt, args...) \
    do {                            \
        if (false)                  \
            printf(fmt, ##args);    \
    } while (false)
#endif

#define sfx_log_info(fmt, args...)  \
        __android_log_print(ANDROID_LOG_INFO, LOG_TAG, "[%s-INF][%s %d] " fmt, AP_CORE, __func__, __LINE__, ##args);

#define sfx_log_warn(fmt, args...)  \
        __android_log_print(ANDROID_LOG_WARN, LOG_TAG, "[%s-WARN][%s %d] " fmt, AP_CORE, __func__, __LINE__, ##args);

#define sfx_log_err(fmt, args...)   \
        __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, "[%s-ERR][%s %d] " fmt, AP_CORE, __func__, __LINE__, ##args);

#define sfx_log_show(fmt, args...)  \
        __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG,  fmt, ##args);

#else
/* Default printing. */
#if SFX_LOG_DEBUG
#define sfx_log_debug(fmt, args...) \
    printf("[%s-DBG][%s %d] " fmt, AP_CORE, __func__, __LINE__, ##args)
#else
#define sfx_log_debug(fmt, args...) \
    do {                            \
        if (false)                  \
            printf(fmt, ##args);    \
    } while (false)
#endif

#define sfx_log_info(fmt, args...)  \
    printf("[%s-INF][%s %d] " fmt, AP_CORE, __func__, __LINE__, ##args)

#define sfx_log_warn(fmt, args...)  \
    printf("[%s-WARN][%s %d] " fmt, AP_CORE, __func__, __LINE__, ##args)

#define sfx_log_err(fmt, args...)   \
    printf("[%s-ERR][%s %d] " fmt, AP_CORE, __func__, __LINE__, ##args)

#define sfx_log_show(fmt, args...)  \
    printf(fmt, ##args)
#endif
#endif

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* __SFX_LOG_H */
