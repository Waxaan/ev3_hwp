
#include <stdio.h>
#include "./ev3/ev3.h"
#include "./ev3/ev3_port.h"
#include "./ev3/ev3_sensor.h"
#include "./ev3/ev3_servo.h"
// WIN32 /////////////////////////////////////////
#ifdef __WIN32__
#include <windows.h>
// UNIX //////////////////////////////////////////
#else
#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )
#endif

int main(void) {
	printf("Hewwo World! UwU\n");
	ev3_init();
	return 0;
}