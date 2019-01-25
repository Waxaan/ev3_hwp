#include <stdio.h>
#include <string.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

static int const TIME = 1000;
static int const PenTIME = 350;
static int const DATALEN = 33;
						// Kette, FahrW. Stift
static float data[33][3] = {{2.544,-11.795,0.0},{1.267,3.165,0.0},{0.01,4.024,0.0},{0.494,-3.538,1.0},{1.646,-0.092,0.0},{1.246,2.257,1.0},{1.969,-0.948,0.0},{0.717,-0.891,0.0},{-18.03,0.476,0.0},{2.402,0.969,0.0},{-0.603,7.111,1.0},{0.415,-3.613,0.0},{0.465,-2.246,0.0},{14.212,5.692,1.0},{0.305,-3.045,0.0},{-15.418,3.038,1.0},{-4.383,1.348,0.0},{-1.877,3.51,0.0},{0.965,4.556,0.0},{3.499,2.963,0.0},{6.689,1.409,0.0},{7.491,-0.671,0.0},{4.738,-2.704,0.0},{1.618,-5.01,0.0},{-7.825,0.61,0.0},{1.646,-1.81,0.0},{-10.216,1.814,0.0},{1.735,-0.235,0.0},{5.54,2.094,1.0},{2.129,0.373,0.0},{-1.118,1.384,1.0},{1.232,0.418,0.0},{1.465,-0.305,0.0}};

//adding these functions to the scope of the compiler
void set_motor(uint8_t port, int mode, float speed);
void move_pen (uint8_t port, char *buf, int maxSpeed);
void reset_motors(uint8_t port1, uint8_t port2, uint8_t port3);



int main( void )
{
	uint8_t Kette, Fahrwerk, Stift;
	int maxKette, maxFahrwerk, maxStift;
	char s[ 256 ];

	if ( ev3_init() == -1 ) return ( 1 );
	while ( ev3_tacho_init() < 1 ) Sleep( 1000 );

	printf( "*** ( EV3 ) Hello! ***\n" );

	//setze die Ports und Geschindigkeit der Kette, Fahrwerk und des Stifts
	if (ev3_search_tacho( LEGO_EV3_L_MOTOR, &Kette,		0) &&
		ev3_search_tacho( LEGO_EV3_L_MOTOR, &Fahrwerk,	1) &&
		ev3_search_tacho( LEGO_EV3_M_MOTOR, &Stift,		0)) {

		printf("Alles gefunden!\n");

		get_tacho_max_speed( Kette, &maxKette);
		get_tacho_max_speed( Fahrwerk, &maxFahrwerk);
		get_tacho_max_speed( Stift, &maxStift);
		maxKette /= 50;
		maxFahrwerk /= 50;
		maxStift /= 50;

	} else { printf("Mindestens einen Motor nicht gefunden!\n"); return(1);}
	
	//hebe Stift zu beginn des Laufs
	move_pen(Stift, "lift", maxStift);
	
	for(int i = 0; i < DATALEN; i++) {

		//Stift senken, falls aufgesetzt werden soll
		if(data[i][3] != 0.0) { move_pen(Stift, "down", maxStift);}

		//setze Eigenschaften der Ketten und des Fahrwerks
		set_motor(Kette, 	TACHO_HOLD, maxKette 	* data[i][0]);
		set_motor(Fahrwerk, TACHO_HOLD, maxFahrwerk * data[i][1]);
		
		//Lass Kette + Fahrwerk fahren
		set_tacho_command_inx( Kette,	TACHO_RUN_TIMED );
		set_tacho_command_inx( Fahrwerk,TACHO_RUN_TIMED );

		Sleep(TIME);

		if(data[i][3] != 0.0) { move_pen(Stift, "lift", maxStift);}
	}
	move_pen(Stift, "lift", maxStift);
 
	reset_motors(Kette, Fahrwerk, Stift);
	ev3_uninit();
	printf( "*** ( EV3 ) Bye! ***\n" );

	return ( 0 );
}

void move_pen(uint8_t port, char *buf, int maxSpeed) {
	set_tacho_stop_action_inx( 	port, TACHO_HOLD );

	if (strcmp("lift", buf) == 0) {
		set_tacho_speed_sp( port, maxSpeed * (-25));
	} else if (strcmp("down", buf) == 0) {
		set_tacho_speed_sp( port, maxSpeed * (25));
	}
	set_tacho_time_sp( 	   port, PenTIME );
	set_tacho_command_inx( port, TACHO_RUN_TIMED );
	Sleep(PenTIME);
}

void set_motor(uint8_t port, int mode, float speed) {
	set_tacho_stop_action_inx( 	port, mode );
	set_tacho_speed_sp( 		port, (int)speed);
	set_tacho_time_sp( 			port, TIME );
}

void reset_motors(uint8_t port1, uint8_t port2, uint8_t port3) {
	set_tacho_command_inx(port1, TACHO_RESET);
	set_tacho_command_inx(port2, TACHO_RESET);
	set_tacho_command_inx(port3, TACHO_RESET);
}