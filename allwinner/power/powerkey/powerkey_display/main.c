/*
 * Copyright (C) 2016 The AllWinner Project
 */

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include "powerkey_display.h"

int main(int argc, char **argv)
{
	printf("powerkey display\n");
	powerkey_display_init();
}
