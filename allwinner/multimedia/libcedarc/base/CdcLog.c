/*
 * =====================================================================================
 *
 *       Filename:  cdc_log.c
 *
 *    Description: Log debug module
 *
 *        Version:  1.0
 *        Created:  2020年01月08日 19时39分38秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (),
 *        Company: AllwinnerTech
 *
 * =====================================================================================
 */
#include <stdio.h>
#include <stdarg.h>
#include "cdc_log.h"
#include <stdlib.h>
#ifdef __ANDROID__
#include <cutils/properties.h>
#endif

enum CDX_LOG_LEVEL_TYPE GLOBAL_LOG_LEVEL = LOG_LEVEL_INFO;

#ifndef __ANDROID__
const char *CDX_LOG_LEVEL_NAME[] = {
     "",
     "",
     [LOG_LEVEL_VERBOSE] = "VERBOSE",
     [LOG_LEVEL_DEBUG]   = "DEBUG  ",
     [LOG_LEVEL_INFO]    = "INFO   ",
     [LOG_LEVEL_WARNING] = "WARNING",
     [LOG_LEVEL_ERROR]   = "ERROR  ",
};
#endif
void CdcGetpropConfigParam(const char *key, int *value)
{
#ifdef __ANDROID__
    char val[256];
    char prefix_default[256] = "cdc.debug";
    char prefix_vendor[256] = "vendor.cdc.debug";
    int ret;

    sprintf(prefix_default, "%s.%s", prefix_default, key);
    ret = property_get(prefix_default, val, NULL);
    if(ret > 0 && atoi(val) >= 0 && atoi(val) <= 1)
    {
        *value = atoi(val);
        logw("Reset log level to %u from property of %s", atoi(val), prefix_default);
    } else if (ret <= 0) {
        sprintf(prefix_vendor, "%s.%s", prefix_vendor, key);
        ret = property_get(prefix_vendor, val, NULL);
        if(ret > 0 && atoi(val) >= 0 && atoi(val) <= 1)
        {
            *value = atoi(val);
            logw("Reset log level to %u from property of %s", atoi(val), prefix_vendor);
        }
    }
#else
    CEDARC_UNUSE(key);
    CEDARC_UNUSE(value);
#endif
}

//TODO: set module tag in log
void log_set_level(unsigned level)
{
    logi("Set log level to %u from /vendor/etc/cedarc.conf", level);
#ifdef __ANDROID__
    char val[256];
    property_get("vendor.omx.debuglevel", val, "3");
    if(atoi(val) >= LOG_LEVEL_VERBOSE && atoi(val) <= LOG_LEVEL_ERROR)
    {
       level = atoi(val);
       logw("Reset log level to %u from property of vendor.cedarc.debuglevel", level);
    }
#endif
    GLOBAL_LOG_LEVEL = level;
}
