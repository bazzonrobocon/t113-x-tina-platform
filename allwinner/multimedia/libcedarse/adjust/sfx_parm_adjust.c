// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * sfx_parm_adjust.c -- sfx for param adjust
 *
 * Dby <dby@allwinnertech.com>
 * lijingpsw <lijingpsw@allwinnertech.com>
 *
 * Copyright (c) 2023 Allwinnertech Ltd.
 */

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <inttypes.h>

/* #include "parm_server.h" */

#define SERVER_NAME "@sfxmgmt_parm_adjust_server"

#define SFX_DEV_NAME_LEN_MAX        32
#define SFX_AP_NMAE_LEN_MAX         32
#define SFX_AP_SUBID_LEN_MAX        32
#define SFX_PARM_FILE_LEN_MAX       128

struct sfx_parm_server {
    char dev_name[SFX_DEV_NAME_LEN_MAX];
    char ap_name[SFX_AP_NMAE_LEN_MAX];
    char ap_sub_id[SFX_AP_SUBID_LEN_MAX];
    char parm_file_name[SFX_PARM_FILE_LEN_MAX];
};

struct sfx_parm_adjust_remount {
    int sockfd;
    socklen_t len;
    struct sockaddr_un address;
};

static int parm_adjust_init(struct sfx_parm_adjust_remount *adjust)
{
    int ret;
    struct timeval recv_timeout = {3, 0};

    adjust->sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    adjust->address.sun_family = AF_UNIX;
    strcpy(adjust->address.sun_path, SERVER_NAME);
    adjust->address.sun_path[0] = 0;    /* abstract namespace */
    adjust->len = strlen(SERVER_NAME) + offsetof(struct sockaddr_un, sun_path);

    ret = connect(adjust->sockfd, (struct sockaddr*)&adjust->address, adjust->len);
    if(ret == -1) {
        printf("connect parm adjust server failed\n");
        return -1;
    }

    ret = setsockopt(adjust->sockfd, SOL_SOCKET, SO_RCVTIMEO,
                     (char *)&recv_timeout, sizeof(struct timeval));
    if(ret == -1) {
        printf("setsockopt failed\n");
        return -1;
    }

    return 0;
}

void parm_adjust_destroy(struct sfx_parm_adjust_remount *adjust)
{
    if (adjust->sockfd)
        close(adjust->sockfd);
}

static int parm_adjust_send(struct sfx_parm_adjust_remount *adjust,
                            struct sfx_parm_server *parm_server)
{
    int32_t ack;    /* must int32, keep consistenrt with params server. */

    if (!adjust || !parm_server) {
        printf("adjust or parm_server is null\n");
        return -1;
    }

    printf("dev type  -> %s\n", parm_server->dev_name);
    printf("ap        -> %s\n", parm_server->ap_name);
    printf("ap sub id -> %s\n", parm_server->ap_sub_id);
    printf("parm file -> %s\n", parm_server->parm_file_name);

    write(adjust->sockfd, parm_server, sizeof(struct sfx_parm_server));

    read(adjust->sockfd, &ack, sizeof(ack));
    if (ack == 0)
        printf("sfx parm adjust send success\n");
    else
        printf("sfx parm adjust send failed\n");

    return ack;
}

int main(int argc, char **argv)
{
    int ret;
    struct sfx_parm_adjust_remount adjust;
    struct sfx_parm_server parm_server;

    if (argc < 7) {
        printf("Usage: %s {-DEV dev name} {-PARM sound effect name} "
               "[-SUBID sound effect id] {-FILE parm file path}\n", argv[0]);
        return 0;
    }

    memset(&parm_server, 0, sizeof(struct sfx_parm_server));

    while (*argv) {
        if (strcmp(*argv, "-DEV") == 0) {
            argv++;
            if(*argv) {
                if (strlen(*argv) <= SFX_DEV_NAME_LEN_MAX) {
                    memcpy(parm_server.dev_name, *argv, strlen(*argv));
                } else {
                    printf("-DEV param too long, max %d\n", SFX_DEV_NAME_LEN_MAX);
                }
            }
        }

        if (strcmp(*argv, "-PARM") == 0) {
            argv++;
            if (*argv) {
                if (strlen(*argv) <= SFX_AP_NMAE_LEN_MAX) {
                    memcpy(parm_server.ap_name, *argv, strlen(*argv));
                } else {
                    printf("-PARM param too long, max %d\n", SFX_AP_NMAE_LEN_MAX);
                }
            }
        }

        if (strcmp(*argv, "-SUBID") == 0) {
            argv++;
            if (*argv) {
                if (strlen(*argv) <= SFX_AP_SUBID_LEN_MAX) {
                    memcpy(parm_server.ap_sub_id, *argv, strlen(*argv));
                } else {
                    printf("-SUBID param too long, max %d\n", SFX_AP_SUBID_LEN_MAX);
                }
            }
        }

        if (strcmp(*argv, "-FILE") == 0) {
            argv++;
            if (*argv) {
                if (strlen(*argv) <= SFX_PARM_FILE_LEN_MAX) {
                    memcpy(parm_server.parm_file_name, *argv, strlen(*argv));
                } else {
                    printf("-PARM param too long, max %d\n", SFX_PARM_FILE_LEN_MAX);
                }
            }
        }

        if (*argv)
            argv++;
    }

    ret = parm_adjust_init(&adjust);
    if (ret) {
        printf("parm_adjust_init failed\n");
        return -1;
    }

    ret = parm_adjust_send(&adjust, &parm_server);
    if (ret) {
        printf("parm_adjust_send failed\n");
        return -1;
    }

    parm_adjust_destroy(&adjust);

    return 0;
}
