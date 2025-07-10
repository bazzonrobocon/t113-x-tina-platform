/*
 * Copyright (C) 2022  Allwinnertech
 * Author: yajianz <yajianz@allwinnertech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"
#include "fcm_types.h"
//#ifdef FCM_10BIT_ENABLE
//const fcm_table_t constants_fcm_table = {
//
//    // float Angle_HUE[28];
//    { 0, 364, 728, 1092, 1456, 1820, 2275, 2731, 3186, 3732, 4460, 5188, 5916, 6644, 7372, 8100,
//      8829, 9557, 10285, 11104, 11923, 12742, 13470, 14108, 14654, 15200, 15655, 16019, },
//    // float Angle_SAT[13];
//    { 0, 491, 819, 1147, 1474, 1802, 2129, 2457, 2785, 3112, 3440, 3767, 4095, },
//    // float Angle_LUM[13];
//    { 0, 123, 205, 286, 368, 450, 532, 614, 696, 777, 859, 941, 1023, },
//    // fl0at HBH_MaxGain[28];
//    {
//        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    },
//    // float SBH_MaxGain[28];
//    {
//        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    },
//    // float YBH_MaxGain[28];
//    {
//        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    },
//
//    /* float SATHBH_Protect[13] */ { 127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127, },
//    /* float LUMHBH_Protect[13] */ { 127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127, },
//    /* float SATSBH_Protect[13] */ { 127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127, },
//    /* float LUMSBH_Protect[13] */ { 127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127, },
//    /* float SATYBH_Protect[13] */ { 127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127, },
//    /* float LUMYBH_Protect[13] */ { 127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127,  127, },
//
//    // int32_t Tone_Enhance_HBH[28];
//    { 0, 0, 0, 0, 0, 0, 0, 0, 25, 40, 60, 30, 0, 0, 0, 27, 40, 50, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//    // int32_t Tone_Enhance_SBH[28];
//    { 0, 11, 20, 18, 11, 0, 0, 0, 0, 20, 29, 44, 27, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
//    // int32_t Tone_Enhance_YBH[28];
//    { 0, 40, 70, 60, 40, 30, 0, 0, 0, 0, 0, 0, 0, 60, 80, 70, 60, 40, 30, 20, 18, 0, 0, 0, 0, 0, 0, 0 },
//
//    // int32_t Hue_MaxNum;
//    // int32_t Sat_MaxNum;
//    // int32_t Lum_MaxNum;
//    HUE_MAX_NUM, SAT_MAX_NUM, LUM_MAX_NUM,
//
//    // int32_t Zero_Angle_Pos;
//    10,
//};
//#else
const fcm_table_t constants_fcm_table = {

    // float Angle_HUE[28];
    {
        226.0f, 244.0f, 262.0f, 280.0f, 296.0f, 310.0f, 322.0f, 334.0f, 344.0f, 352.0f,   0.0f,   8.0f,  16.0f,  24.0f,
        32.0f,  40.0f,   50.0f,  60.0f,  70.0f,  82.0f,  98.0f, 114.0f, 130.0f, 146.0f, 162.0f, 178.0f, 194.0f, 210.0f,
    },
    // float Angle_SAT[13];
    { 0.0f, 0.12f, 0.20f, 0.28f, 0.36f, 0.44f, 0.52f, 0.60f, 0.68f, 0.76f, 0.84f, 0.92f, 1.0f, },
    // float Angle_LUM[13];
    { 0.0f, 0.12f, 0.20f, 0.28f, 0.36f, 0.44f, 0.52f, 0.60f, 0.68f, 0.76f, 0.84f, 0.92f, 1.0f, },

    // fl0at HBH_MaxGain[28];
    {
        30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 30.0f, 25.0f, 25.0f, 20.0f, 20.0f, 20.0f, 20.0f,
        25.0f, 25.0f, 25.0f, 30.0f, 35.0f, 35.0f, 35.0f, 35.0f, 35.0f, 35.0f, 32.0f, 32.0f, 32.0f, 32.0f,
    },
    // float SBH_MaxGain[28];
    {
        0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.22f, 0.22f, 0.2f,  0.20f, 0.20f, 0.20f,
        0.22f, 0.22f, 0.22f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f,
    },
    // float YBH_MaxGain[28];
    {
        0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.22f, 0.22f, 0.2f,  0.20f, 0.20f, 0.20f,
        0.22f, 0.22f, 0.22f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f,
    },

    /* float SATHBH_Protect[13] */ { 0.78,  0.78,  0.84,  0.84,  0.90,  0.94,  0.96,  0.98,  0.98,  0.98,  0.98,  1.00,  1.00, },
    /* float LUMHBH_Protect[13] */ { 0.78, 0.78, 0.82, 0.86, 0.90, 0.94, 0.96, 0.98, 0.98, 0.98, 0.98, 1.00, 1.00 },
    /* float SATSBH_Protect[13] */ { 0.00, 0.31, 0.63, 0.78, 0.94, 0.98, 1.00, 1.00, 1.00, 1.00, 0.98, 0.96, 0.94, },
    /* float LUMSBH_Protect[13] */ { 0.00, 0.31, 0.63, 0.78, 0.94, 0.98, 1.00, 1.00, 1.00, 1.00, 0.98, 0.96, 0.94, },
    /* float SATYBH_Protect[13] */ { 0.00, 0.16, 0.26, 0.55, 0.78, 0.94, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, },
    /* float LUMYBH_Protect[13] */ { 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, },

    // int32_t Tone_Enhance_HBH[28];
    { 0, 0, 0, 0, 0, 0, 0, 0, 25, 40, 60, 30, 0, 0, 0, 27, 40, 50, 60, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    // int32_t Tone_Enhance_SBH[28];
    { 0, 11, 20, 18, 11, 0, 0, 0, 0, 20, 29, 44, 27, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    // int32_t Tone_Enhance_YBH[28];
    { 0, 40, 70, 60, 40, 30, 0, 0, 0, 0, 0, 0, 0, 60, 80, 70, 60, 40, 30, 20, 18, 0, 0, 0, 0, 0, 0, 0 },

    // int32_t Hue_MaxNum;
    // int32_t Sat_MaxNum;
    // int32_t Lum_MaxNum;
    HUE_MAX_NUM, SAT_MAX_NUM, LUM_MAX_NUM,

    // int32_t Zero_Angle_Pos;
    10,
};
//#endif
