/*
 * Copyright (C) 2016 The AllWinner Project
 */

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "powerkey_suspend.h"

int main(int argc, char **argv)
{
	printf("powerkey suspend\n");
	powerkey_suspend_init();
}
