/*
 * Copyright (C) 2016 The AllWinner Project
 */
#include <pthread.h>
#include <linux/input.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <glob.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <poll.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define MSEC_PER_SEC            (1000LL)
#define NSEC_PER_MSEC           (1000000LL)
#define LOG_TAG "powerkey"

static pthread_t thread_key_power;
static pthread_mutex_t sleep_lock = PTHREAD_MUTEX_INITIALIZER;

static int powerkey_event(const char * eventDevName)//eventDevname by !cat has '\n' terminated!
{
	const char* TPdevName = "axp2101-pek";
	int i = 0;
	const int eventDevLen = strlen(eventDevName);
	const int TPDevLen    = strlen(TPdevName);
	for (i = 0; i < TPDevLen; i++) {
		const char srcVal = TPdevName[TPDevLen - 1 - i ];
		const char devVal = eventDevName[eventDevLen - 2 - i];
		if(srcVal != devVal)
			return 0;
	}
	return 1;
}

const char* get_event_file_path(void)
{
	char devNameNod[] = "/sys/class/input/event0/device/name";
	int i =0 ;
	FILE* fp = NULL;
	static char evtPath[] = "/dev/input/event0";
	const char* devFilePath = NULL;
	char eventDevName[32];
	for (i = 0; i < 20 && !devFilePath;
		i++, devNameNod[strlen(devNameNod) - 1 - 12] += 1) {
		fp = fopen(devNameNod, "r");
		if(!fp){
			printf("fail to open %s\n", devNameNod);
			break;
		}
		if(!fgets(eventDevName, 32, fp))
		{
			printf("cann't read from %s(error:%s)\n", devNameNod, strerror(errno));
			fclose(fp);
			break;
		}
		if(powerkey_event(eventDevName) )
		{
			evtPath[strlen(evtPath) - 1] += i;
			devFilePath = evtPath;
		}
		fclose(fp);
	}
	return devFilePath;
}

static uint64_t curr_time_ms(void)
{
	struct timespec tm;
	clock_gettime(CLOCK_MONOTONIC, &tm);
	return tm.tv_sec * MSEC_PER_SEC + (tm.tv_nsec / NSEC_PER_MSEC);
}

/*=============================================================
 * Function: Scan keys to enter black screen/bright screen
 *============================================================*/
static void *scanPowerKey(void *arg)
{
	uint64_t suspend_time_ms = 0;
	uint64_t powerkey_down_ms = 0;
	int input_event_count = 128;
	int nevents, timeout = -1;
	int input_event_fd[input_event_count];
	int i, sysret;
	int press_time = 0;
	struct epoll_event ev;
	struct epoll_event events_inputs[input_event_count];
	struct input_event *key_event;
	char buffer[128];
	const char *eventfile = NULL;
	eventfile = get_event_file_path();
	if(!eventfile){
		printf("Fail to get event file\n");
		return 0;
	}
	printf("event file is %s\n", eventfile);
	int fd = open(eventfile, O_RDONLY | O_NONBLOCK);
	if (fd == -1) {
	 	printf("open file failed.\n");
		return NULL;
	}
	int epollfd = epoll_create(10);
	if (epollfd == -1) {
	 	printf("epoll create failed.\n");
		return NULL;
	}
	ev.data.fd = fd;
	ev.events = EPOLLIN | EPOLLWAKEUP;
	if (epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &ev)) {
		printf("epoll_ctl failed.\n");
		return NULL;
	}
	key_event = (struct input_event*)malloc(sizeof(struct input_event));
	while (1) {
		memset(&events_inputs, 0, sizeof(events_inputs));
		nevents = epoll_wait(epollfd, events_inputs, input_event_count, timeout);
			for(i = 0; i < 2; i++){
				int err = read(fd, key_event, sizeof(struct input_event));
				if(err == -1){
				 	printf("read power_key input error\n");
				}
				if(key_event->value == 1 && key_event->type == 1 && key_event->code == KEY_POWER){
				 	press_time = curr_time_ms();
				}
				if(key_event->value == 0 && key_event->type == 1 && key_event->code == KEY_POWER){
					suspend_time_ms = curr_time_ms();
				 	if(suspend_time_ms - press_time < 500 && suspend_time_ms - powerkey_down_ms > 300 ){
				 		sysret = system("echo mem > /sys/power/state");
						powerkey_down_ms = curr_time_ms();
					}
				}
			}
	}
	free(key_event);
}
int powerkey_suspend_init()
{
	int res = 0;
	res = pthread_create(&thread_key_power, NULL, scanPowerKey, NULL);
	if (res < 0) {
		printf("\tcreate thread of scanPowerKey failed\n");
		return -1;
	}
	pthread_join(thread_key_power,NULL);
	return res;
}
