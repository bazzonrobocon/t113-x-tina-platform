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

#ifndef _pq_de35x_h_
#define _pq_de35x_h_

#define DCI_REG_COUNT 57
#define DEBAND_REG_COUNT 22
#define TNR_REG_COUNT 27
#define SHARP_DE35x_COUNT 26
#define HDR_REG_COUNT 6
#define SNR_REG_COUNT 11

typedef struct _dci_module_param_t{
    int id; //20
    unsigned int value[DCI_REG_COUNT];

}dci_module_param_t;

typedef struct _deband_module_param_t{
    int id; //21
    unsigned int value[DEBAND_REG_COUNT];

}deband_module_param_t;

typedef struct _sharp_de35x_t{
    int id; //22
    unsigned int value[SHARP_DE35x_COUNT];

}sharp_de35x_t;

typedef struct _tnr_module_param_t{
    int id; //23
    unsigned int value[TNR_REG_COUNT];
}tnr_module_param_t;

typedef struct _hdr_module_param_t{
    int id; //24
    unsigned int value[HDR_REG_COUNT];
}hdr_module_param_t;

typedef struct _snr_module_param_t{
    int id; //25
    unsigned int value[SNR_REG_COUNT];
}snr_module_param_t;

int de35x_set_deband(const char* data, int size);
int de35x_get_deband(char* data, int size);

int de35x_set_sharp(const char* data, int size);
int de35x_get_sharp(char* data, int size);

int de35x_set_dci(const char *data, int size);
int de35x_get_dci(char *data, int size);

int de35x_set_gtm(const char *data, int size);
int de35x_get_gtm(char *data, int size);

int de35x_set_snr(const char *data, int size);
int de35x_get_snr(char *data, int size);

int de35x_set_tnr(const char *data, int size);
int de35x_get_tnr(char *data, int size);

int de35x_set_gamma_lut(const char *data, int size);
int de35x_get_gamma_lut(char *data, int size);
#endif

