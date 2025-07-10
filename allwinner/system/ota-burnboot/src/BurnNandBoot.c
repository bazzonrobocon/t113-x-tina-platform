/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>

#include "Utils.h"
#include "BurnNandBoot.h"

#define NAND_BLKBURNBOOT0		_IO('v',127)
#define NAND_BLKBURNUBOOT		_IO('v',128)
#define DEVNODE_PATH_NAND_MBR		"/dev/nanda"
#define DEVNODE_PATH_NAND_GPT		"/dev/nand0p1"
#define DEVNODE_PATH_NAND_MTD_BOOT0	"/dev/mtd0"
#define DEVNODE_PATH_NAND_MTD_BOOT1	"/dev/mtd1"

//use this file to update nand boot and uboot, when not use ioctl
#define DEVNODE_PATH_NAND_CDEV      "/dev/rawnand_cdev"

void clearPageCache(){
	FILE *fp = fopen("/proc/sys/vm/drop_caches", "w+");
	if (fp == NULL) {
		ob_error("open /proc/sys/vm/drop_caches fail\n");
		return;
	}
	char *num = "1";
	fwrite(num, sizeof(char), 1, fp);
	fclose(fp);
}

int burnNandBoot0(BufferExtractCookie *cookie) {

	int fd = -1;
	int ret = 0;
	char* dev;
	struct burn_param_t boot0_para;
	if (checkBoot0Sum(cookie)) {
		ob_error("wrong boot0 binary file!\n");
		return -1;
	}

	if (check_is_ubi())
		dev = DEVNODE_PATH_NAND_MTD_BOOT0;
	else if (check_is_gpt())
		dev = DEVNODE_PATH_NAND_GPT;
	else
		dev = DEVNODE_PATH_NAND_MBR;

	//if have this file, use this to update, not use ioctrl
	if (access(DEVNODE_PATH_NAND_CDEV, F_OK) != -1) {
		ob_debug("use %s to update boot0.\n");
		boot0_para.type = BOOT0;
		boot0_para.length = cookie->len;
		boot0_para.buffer = cookie->buffer;

		dev = DEVNODE_PATH_NAND_CDEV;
	}

	fd = open(dev, O_RDWR);
	if (fd == -1) {
		ob_error("open device node %s failed ! errno is %d : %s\n", dev, errno, strerror(errno));
		return -1;
	}

	clearPageCache();

	if (access(DEVNODE_PATH_NAND_CDEV, F_OK) != -1) {
		ob_debug("boot0_para.length:%d, sizeof(struct burn_param_t):%u\n", boot0_para.length, sizeof(struct burn_param_t));
		ret = write(fd, &boot0_para, sizeof(struct burn_param_t));
		if (ret < 0) {
			ob_error("burnNandBoot0 failed ! errno is %d : %s\n", errno, strerror(errno));
		} else {
			ob_debug("burnNandBoot0 succeed!\n");
			ret = 0;
		}
	} else {
		ret = ioctl(fd, NAND_BLKBURNBOOT0, (unsigned long)cookie);
		if (ret)
			ob_error("burnNandBoot0 failed ! errno is %d : %s\n", errno, strerror(errno));
		else
			ob_debug("burnNandBoot0 succeed!\n");
	}

	close(fd);

	return ret;
}

int burnNandUboot(BufferExtractCookie *cookie) {

	int fd = -1;
	int ret;
	char *dev;
	struct burn_param_t uboot_para;
	if (checkUbootSum(cookie)) {
		ob_error("wrong uboot binary file!\n");
		return -1;
	}

	if (check_is_ubi())
		dev = DEVNODE_PATH_NAND_MTD_BOOT1;
	else if (check_is_gpt())
		dev = DEVNODE_PATH_NAND_GPT;
	else
		dev = DEVNODE_PATH_NAND_MBR;

	//if have this file, use this to update, not use ioctrl
	if (access(DEVNODE_PATH_NAND_CDEV, F_OK) != -1) {
		ob_debug("use %s to update boot0.\n");
		uboot_para.type = UBOOT;
		uboot_para.length = cookie->len;
		uboot_para.buffer = cookie->buffer;

		dev = DEVNODE_PATH_NAND_CDEV;
	}
	fd = open(dev, O_RDWR);
	if (fd == -1) {
		ob_error("open device node %s failed ! errno is %d : %s\n", dev, errno, strerror(errno));
		return -1;
	}

	clearPageCache();

	if (access(DEVNODE_PATH_NAND_CDEV, F_OK) != -1) {
		ob_debug("uboo0_para.length:%d, sizeof(struct burn_param_t):%u\n", uboot_para.length, sizeof(struct burn_param_t));
		ret = write(fd, &uboot_para, sizeof(struct burn_param_t));
		if (ret < 0) {
			ob_error("burnNandUboot failed ! errno is %d : %s\n", errno, strerror(errno));
		} else {
			ob_debug("burnNandUboot succeed!\n");
			ret = 0;
		}
	} else {
	ret = ioctl(fd, NAND_BLKBURNUBOOT, (unsigned long)cookie);

	if (ret)
		ob_error("burnNandUboot failed ! errno is %d : %s\n", errno, strerror(errno));
	else
		ob_debug("burnNandUboot succeed!\n");
	}

	close(fd);

	return ret;
}
