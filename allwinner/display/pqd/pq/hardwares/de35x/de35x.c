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

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "debug.h"
#include "fcm/fcm.h"
#include "hardwares/driver_api.h"
#include "pqal.h"
#include "sockets/protocol.h"
#include "pqdata/config.h"
#include "de35x.h"

#define DI_IOC_MAGIC 'D'
#define DI_IOR(nr, size)   _IOR(DI_IOC_MAGIC, nr, size)
#define DI_IOW(nr, size)   _IOW(DI_IOC_MAGIC, nr, size)

#define DI_IOC_GET_TNRPARA    DI_IOR(0x13, tnr_module_param_t)
#define DI_IOC_SET_TNRPARA    DI_IOW(0x14, tnr_module_param_t)

static int get_dispdev_fd(void) {
    static int devfd = -1;
    if (devfd < 0) {
        devfd = open("/dev/disp", O_RDWR);
    }
    return devfd;
}

static int get_didev_fd(void) {
    static int devfd = -1;
    if (devfd < 0) {
        devfd = open("/dev/deinterlace", O_RDWR);
    }
    return devfd;
}

int de35x_set_sharp(const char* data, int size)
{
    const struct hardware_data* hwdat = pq_hardware_data();
    unsigned long arg[4] = {0};

    if (size < sizeof(sharp_de35x_t)) {
        dloge("packet size error, size=%d sizeof(para)=%zu", size, sizeof(sharp_de35x_t));
        return -1;
    }
    dloge("sharp35x packet size=%d sizeof(para)=%zu", size, sizeof(sharp_de35x_t));

    sharp_de35x_t* sharp = malloc(sizeof(sharp_de35x_t));
    memcpy(sharp, data, sizeof(sharp_de35x_t));

    for (int i = 0; i < SHARP_DE35x_COUNT; i++) {
        dloge("sharp35x sharp->value[%d]=%d", i, sharp->value[i]);
    }
    arg[0] = (unsigned long)PQ_SHARP35X;
    arg[1] = (unsigned long)hwdat->port;
    arg[2] = (unsigned long)sharp;
    arg[3] = (unsigned long)sizeof(sharp_de35x_t);

    int ret = ioctl(get_dispdev_fd(), DISP_PQ_PROC, (unsigned long)arg);
    if (ret) {
        dloge("DISP_PQ_PROC PQ_SHARP35X failed: %d port=%d", ret, hwdat->port);
        return -1;
    }

    free(sharp);
    return 0;
}

int de35x_get_sharp(char* data, int size)
{
    const struct hardware_data* hwdat = pq_hardware_data();
    unsigned long arg[4] = {0};

    if (size < sizeof(sharp_de35x_t)) {
        dloge("packet size error, size=%d sizeof(para)=%zu", size, sizeof(sharp_de35x_t));
        return -1;
    }
    dloge("sharp35x packet size=%d sizeof(para)=%zu", size, sizeof(sharp_de35x_t));

    sharp_de35x_t* sharp = malloc(sizeof(sharp_de35x_t));
    memset(sharp, 0, sizeof(sharp_de35x_t));
    sharp->id = DE35X_SHARP_CTRL;

    for (int i = 0; i < SHARP_DE35x_COUNT; i++) {
        dloge("sharp35x sharp->value[%d]=%d", i, sharp->value[i]);
    }
    arg[0] = (unsigned long)PQ_SHARP35X;
    arg[1] = (unsigned long)hwdat->port | (1 << 4);
    arg[2] = (unsigned long)sharp;
    arg[3] = (unsigned long)sizeof(sharp_de35x_t);

    int ret = ioctl(get_dispdev_fd(), DISP_PQ_PROC, (unsigned long)arg);
    if (ret) {
        dloge("DISP_PQ_PROC PQ_SHARP35X failed: %d port=%d", ret, hwdat->port);
        return -1;
    }

    for (int i = 0; i < SHARP_DE35x_COUNT; i++) {
        dloge("sharp35x sharp->value[%d]=%d", i, sharp->value[i]);
    }
    memcpy(data, sharp, sizeof(sharp_de35x_t));
    free(sharp);
    return 0;

}

int de35x_set_dci(const char* data, int size)
{
    const struct hardware_data* hwdat = pq_hardware_data();
    unsigned long arg[4] = {0};

    if (size < sizeof(dci_module_param_t)) {
        dloge("packet size error, size=%d sizeof(para)=%zu", size, sizeof(dci_module_param_t));
        return -1;
    }
    dloge("dci packet size=%d sizeof(para)=%zu", size, sizeof(dci_module_param_t));

    dci_module_param_t* dci = malloc(sizeof(dci_module_param_t));
    memcpy(dci, data, sizeof(dci_module_param_t));

    for (int i = 0; i < DCI_REG_COUNT; i++) {
        dloge("dci->value[%d]=%d", i, dci->value[i]);
    }
    arg[0] = (unsigned long)PQ_DCI;
    arg[1] = (unsigned long)hwdat->port;
    arg[2] = (unsigned long)dci;
    arg[3] = (unsigned long)sizeof(dci_module_param_t);

    int ret = ioctl(get_dispdev_fd(), DISP_PQ_PROC, (unsigned long)arg);
    if (ret) {
        dloge("DISP_PQ_PROC PQ_DCI failed: %d port=%d", ret, hwdat->port);
        return -1;
    }

    free(dci);
    return 0;
}

int de35x_get_dci(char* data, int size)
{
    const struct hardware_data* hwdat = pq_hardware_data();
    unsigned long arg[4] = {0};

    if (size < sizeof(dci_module_param_t)) {
        dloge("packet size error, size=%d sizeof(para)=%zu", size, sizeof(dci_module_param_t));
        return -1;
    }
    dloge("dci packet size=%d sizeof(para)=%zu", size, sizeof(dci_module_param_t));

    dci_module_param_t* dci = malloc(sizeof(dci_module_param_t));
    memset(dci, 0, sizeof(dci_module_param_t));
    dci->id = DCI_CTRL;

    for (int i = 0; i < DCI_REG_COUNT; i++) {
        dloge("dci->value[%d]=%d", i, dci->value[i]);
    }
    arg[0] = (unsigned long)PQ_DCI;
    arg[1] = (unsigned long)hwdat->port | (1 << 4);
    arg[2] = (unsigned long)dci;
    arg[3] = (unsigned long)sizeof(dci_module_param_t);

    int ret = ioctl(get_dispdev_fd(), DISP_PQ_PROC, (unsigned long)arg);
    if (ret) {
        dloge("DISP_PQ_PROC PQ_DCI failed: %d port=%d", ret, hwdat->port);
        return -1;
    }

    for (int i = 0; i < DCI_REG_COUNT; i++) {
        dloge("dci->value[%d]=%d", i, dci->value[i]);
    }
    memcpy(data, dci, sizeof(dci_module_param_t));
    free(dci);
    return 0;

}

int de35x_set_deband(const char* data, int size)
{
    const struct hardware_data* hwdat = pq_hardware_data();
    unsigned long arg[4] = {0};

    if (size < sizeof(deband_module_param_t)) {
        dloge("packet size error, size=%d sizeof(para)=%zu", size, sizeof(deband_module_param_t));
        return -1;
    }
    dloge("deband packet size=%d sizeof(para)=%zu", size, sizeof(deband_module_param_t));

    deband_module_param_t* deband = malloc(sizeof(deband_module_param_t));
    memcpy(deband, data, sizeof(deband_module_param_t));

    for (int i = 0; i < DEBAND_REG_COUNT; i++) {
        dloge("deband->value[%d]=%d", i, deband->value[i]);
    }
    arg[0] = (unsigned long)PQ_DEBAND;
    arg[1] = (unsigned long)hwdat->port;
    arg[2] = (unsigned long)deband;
    arg[3] = (unsigned long)sizeof(deband_module_param_t);

    int ret = ioctl(get_dispdev_fd(), DISP_PQ_PROC, (unsigned long)arg);
    if (ret) {
        dloge("DISP_PQ_PROC PQ_DEBAND failed: %d port=%d", ret, hwdat->port);
        return -1;
    }

    free(deband);
    return 0;
}

int de35x_get_deband(char* data, int size)
{
    const struct hardware_data* hwdat = pq_hardware_data();
    unsigned long arg[4] = {0};

    if (size < sizeof(deband_module_param_t)) {
        dloge("debandsize error, size=%d sizeof(para)=%zu", size, sizeof(deband_module_param_t));
        return -1;
    }
    dloge("deband packet size=%d sizeof(para)=%zu", size, sizeof(deband_module_param_t));

    deband_module_param_t* deband = malloc(sizeof(deband_module_param_t));
    memset(deband, 0, sizeof(deband_module_param_t));
    deband->id = DEBAND_CTRL;

    for (int i = 0; i < DEBAND_REG_COUNT; i++) {
        dloge("deband->value[%d]=%d", i, deband->value[i]);
    }
    arg[0] = (unsigned long)PQ_DEBAND;
    arg[1] = (unsigned long)hwdat->port | (1 << 4);
    arg[2] = (unsigned long)deband;
    arg[3] = (unsigned long)sizeof(deband_module_param_t);

    int ret = ioctl(get_dispdev_fd(), DISP_PQ_PROC, (unsigned long)arg);
    if (ret) {
        dloge("DISP_PQ_PROC PQ_DEBAND failed: %d port=%d", ret, hwdat->port);
        return -1;
    }

    for (int i = 0; i < DEBAND_REG_COUNT; i++) {
        dloge("deband->value[%d]=%d", i, deband->value[i]);
    }
    memcpy(data, deband, sizeof(deband_module_param_t));
    free(deband);
    return 0;
}

int de35x_set_tnr(const char* data, int size)
{
    const struct hardware_data* hwdat = pq_hardware_data();

    if (size < sizeof(tnr_module_param_t)) {
        dloge("packet size error, size=%d sizeof(para)=%zu", size, sizeof(tnr_module_param_t));
        return -1;
    }
    dloge("tnr packet size=%d sizeof(para)=%zu", size, sizeof(tnr_module_param_t));

    tnr_module_param_t* tnr = malloc(sizeof(tnr_module_param_t));
    memcpy(tnr, data, sizeof(tnr_module_param_t));

    for (int i = 0; i < TNR_REG_COUNT; i++) {
        dloge("set tnr->value[%d]=%d", i, tnr->value[i]);
    }

    int ret = ioctl(get_didev_fd(), DI_IOC_SET_TNRPARA, tnr);
    if (ret) {
        dloge("DI_IOC_SET_TNRPARA PQ_TNR failed: %d port=%d", ret, hwdat->port);
        free(tnr);
        return -1;
    }

    free(tnr);
    return 0;
}

int de35x_get_tnr(char* data, int size)
{
    const struct hardware_data* hwdat = pq_hardware_data();

    if (size < sizeof(tnr_module_param_t)) {
        dloge("packet size error, size=%d sizeof(para)=%zu", size, sizeof(tnr_module_param_t));
        return -1;
    }
    dloge("tnr packet size=%d sizeof(para)=%zu", size, sizeof(tnr_module_param_t));

    tnr_module_param_t* tnr = malloc(sizeof(tnr_module_param_t));
    memset(tnr, 0, sizeof(tnr_module_param_t));
    tnr->id = TNR_CTRL;

    for (int i = 0; i < TNR_REG_COUNT; i++) {
        dloge("get tnr->value[%d]=%d", i, tnr->value[i]);
    }

    int ret = ioctl(get_didev_fd(), DI_IOC_GET_TNRPARA, tnr);
    if (ret) {
        dloge("DI_IOC_GET_TNRPARA PQ_TNR failed: %d port=%d", ret, hwdat->port);
        free(tnr);
        return -1;
    }

    for (int i = 0; i < TNR_REG_COUNT; i++) {
        dloge("tnr->value[%d]=%d", i, tnr->value[i]);
    }
    tnr->id = TNR_CTRL;
    memcpy(data, tnr, sizeof(tnr_module_param_t));
    free(tnr);
    return 0;
}

int de35x_set_gtm(const char* data, int size)
{
    const struct hardware_data* hwdat = pq_hardware_data();
    unsigned long arg[4] = {0};

    if (size < sizeof(hdr_module_param_t)) {
        dloge("packet size error, size=%d sizeof(para)=%zu", size, sizeof(hdr_module_param_t));
        return -1;
    }
    dloge("gtm packet size=%d sizeof(para)=%zu", size, sizeof(hdr_module_param_t));

    hdr_module_param_t* gtm = malloc(sizeof(hdr_module_param_t));
    memcpy(gtm, data, sizeof(hdr_module_param_t));

    for (int i = 0; i < HDR_REG_COUNT; i++) {
        dloge("gtm->value[%d]=%d", i, gtm->value[i]);
    }
    arg[0] = (unsigned long)PQ_GTM;
    arg[1] = (unsigned long)hwdat->port;
    arg[2] = (unsigned long)gtm;
    arg[3] = (unsigned long)sizeof(hdr_module_param_t);

    int ret = ioctl(get_dispdev_fd(), DISP_PQ_PROC, (unsigned long)arg);
    if (ret) {
        dloge("DISP_PQ_PROC PQ_GTM failed: %d port=%d", ret, hwdat->port);
        return -1;
    }

    free(gtm);
    return 0;
}

int de35x_get_gtm(char* data, int size)
{
    const struct hardware_data* hwdat = pq_hardware_data();
    unsigned long arg[4] = {0};

    if (size < sizeof(hdr_module_param_t)) {
        dloge("packet size error, size=%d sizeof(para)=%zu", size, sizeof(hdr_module_param_t));
        return -1;
    }
    dloge("gtm packet size=%d sizeof(para)=%zu", size, sizeof(hdr_module_param_t));

    hdr_module_param_t* gtm = malloc(sizeof(hdr_module_param_t));
    memset(gtm, 0, sizeof(hdr_module_param_t));
    gtm->id = GTM_CTRL;

    for (int i = 0; i < HDR_REG_COUNT; i++) {
        dloge("gtm->value[%d]=%d", i, gtm->value[i]);
    }
    arg[0] = (unsigned long)PQ_GTM;
    arg[1] = (unsigned long)hwdat->port | (1 << 4);
    arg[2] = (unsigned long)gtm;
    arg[3] = (unsigned long)sizeof(hdr_module_param_t);

    int ret = ioctl(get_dispdev_fd(), DISP_PQ_PROC, (unsigned long)arg);
    if (ret) {
        dloge("DISP_PQ_PROC PQ_GTM failed: %d port=%d", ret, hwdat->port);
        return -1;
    }

    for (int i = 0; i < HDR_REG_COUNT; i++) {
        dloge("gtm->value[%d]=%d", i, gtm->value[i]);
    }
    memcpy(data, gtm, sizeof(hdr_module_param_t));
    free(gtm);
    return 0;
}

int de35x_set_snr(const char* data, int size)
{
    const struct hardware_data* hwdat = pq_hardware_data();
    unsigned long arg[4] = {0};

    if (size < sizeof(snr_module_param_t)) {
        dloge("packet size error, size=%d sizeof(para)=%zu", size, sizeof(snr_module_param_t));
        return -1;
    }
    dloge("snr packet size=%d sizeof(para)=%zu", size, sizeof(snr_module_param_t));

    snr_module_param_t* snr = malloc(sizeof(snr_module_param_t));
    memcpy(snr, data, sizeof(snr_module_param_t));

    for (int i = 0; i < SNR_REG_COUNT; i++) {
        dloge("snr->value[%d]=%d", i, snr->value[i]);
    }
    arg[0] = (unsigned long)PQ_SNR;
    arg[1] = (unsigned long)hwdat->port;
    arg[2] = (unsigned long)snr;
    arg[3] = (unsigned long)sizeof(snr_module_param_t);

    int ret = ioctl(get_dispdev_fd(), DISP_PQ_PROC, (unsigned long)arg);
    if (ret) {
        dloge("DISP_PQ_PROC PQ_SNR failed: %d port=%d", ret, hwdat->port);
        return -1;
    }

    free(snr);
    return 0;
}

int de35x_get_snr(char* data, int size)
{
    const struct hardware_data* hwdat = pq_hardware_data();
    unsigned long arg[4] = {0};

    if (size < sizeof(snr_module_param_t)) {
        dloge("packet size error, size=%d sizeof(para)=%zu", size, sizeof(snr_module_param_t));
        return -1;
    }
    dloge("snr packet size=%d sizeof(para)=%zu", size, sizeof(snr_module_param_t));

    snr_module_param_t* snr = malloc(sizeof(snr_module_param_t));
    memset(snr, 0, sizeof(snr_module_param_t));
    snr->id = SNR_CTRL;

    for (int i = 0; i < SNR_REG_COUNT; i++) {
        dloge("snr->value[%d]=%d", i, snr->value[i]);
    }
    arg[0] = (unsigned long)PQ_SNR;
    arg[1] = (unsigned long)hwdat->port | (1 << 4);
    arg[2] = (unsigned long)snr;
    arg[3] = (unsigned long)sizeof(snr_module_param_t);

    int ret = ioctl(get_dispdev_fd(), DISP_PQ_PROC, (unsigned long)arg);
    if (ret) {
        dloge("DISP_PQ_PROC PQ_SNR failed: %d port=%d", ret, hwdat->port);
        return -1;
    }

    for (int i = 0; i < SNR_REG_COUNT; i++) {
        dloge("snr->value[%d]=%d", i, snr->value[i]);
    }
    memcpy(data, snr, sizeof(snr_module_param_t));
    free(snr);
    return 0;
}

// ------------------------------------------------------------------------------------------//
//                                     Gamma lut                                             //
// ------------------------------------------------------------------------------------------//

static int packet_to_gamma_lut(const char* data, int size, struct hw_gamma_lut_10bit* out)
{
    if (size < 1024 * 3) {
        dloge("packet size error, convert to hardware gamma lut failed!");
        return -1;
    }

    int i = 0;
    const short int* p = (const short int*)data;
    while (i < 1024) {
        out->x[i] = (((uint32_t)p[i]) << 20) | (((uint32_t)p[i + 1024]) << 10) | (p[i + 2048]);
        /* dloge("DE_GAMMA: set gamma lut[%d]=%d", i, out->x[i]); */
        i++;
    }
    /*
    for (i=0; i < 1024*3; i++) {
        dloge("DE_GAMMA: set gamma data[%d]=%d", i, p[i]);
    }
    */
    return 0;
}

static int gamma_lut_to_packet(const struct hw_gamma_lut_10bit* lut, char* data)
{
    int i = 0;
    short int* p = (short int*)data;
    while (i < 1024) {
        p[i]        = (short int)(lut->x[i] >> 20) & 0x3ff;
        p[i + 1024] = (short int)(lut->x[i] >> 10) & 0x3ff;
        p[i + 2048] = (short int)(lut->x[i]      ) & 0x3ff;
        /* dloge("DE_GAMMA: get gamma lut[%d]=%d", i, lut->x[i]); */
        i++;
    }
    /*
    for (i=0; i < 1024*3; i++) {
        dloge("DE_GAMMA: get gamma data[%d]=%d", i, p[i]);
    }
    */
    return 0;
}

int de35x_set_gamma_lut(const char *data, int size)
{
    const struct payload_head *head = (struct payload_head *)data;
    size -= sizeof(struct payload_head);
    struct hw_gamma_lut_10bit lut = {0};
    packet_to_gamma_lut(head->data, size, &lut);

    unsigned long arg[4] = {0};
    int ret = ioctl(get_dispdev_fd(), DISP_LCD_GAMMA_CORRECTION_ENABLE, arg);
    if (ret) {
        dloge("DISP_LCD_GAMMA_CORRECTION_ENABLE failed: %d", ret);
        return -1;
    }

    const struct hardware_data *hwdat = pq_hardware_data();

    arg[0] = (unsigned long)hwdat->port;
    arg[1] = (unsigned long)&lut;
    arg[2] = (unsigned long)sizeof(lut);

    ret = ioctl(get_dispdev_fd(), DISP_LCD_SET_GAMMA_TABLE, (unsigned long)arg);
    if (ret) {
        dloge("DISP_LCD_GAMMA_TABLE failed: %d", ret);
        return -1;
    }
    return 0;
}

int de35x_get_gamma_lut(char *data, int size)
{
    const struct hardware_data *hwdat = pq_hardware_data();
    struct hw_gamma_lut_10bit lut = {0};
    unsigned long arg[4] = {0};

    UNUSED(size);
    arg[0] = (unsigned long)hwdat->port;
    arg[1] = (unsigned long)&lut;
    arg[2] = (unsigned long)sizeof(lut);

    int ret = ioctl(get_dispdev_fd(), DISP_LCD_GET_GAMMA_TABLE, (unsigned long)arg);
    if (ret) {
        dloge("DISP_LCD_GET_GAMMA_TABLE failed: %d", ret);
        return -1;
    }
    struct payload_head *head = (struct payload_head *)data;
    gamma_lut_to_packet(&lut, head->data);

    return 0;
}
