#include <stdio.h>
#include <string.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

static int const TIME = 1000;
static int const PenTIME = 250;
static int const DATALEN = 4;
						// Kette, FahrW. Stift
static float data[4][3] = {{5.0, 10.0, 1.0}, {5.0, 20.0, 1.0}, {-15.0, 30.0, 0.0}, {5.0, -10.0, 0.0}};

//adding these functions to the scope of the compiler
void set_motor(uint8_t port, int mode, float speed);
void move_pen (uint8_t port, char *buf, int maxSpeed);



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
 
	ev3_uninit();
	printf( "*** ( EV3 ) Bye! ***\n" );

	return ( 0 );
}

void move_pen(uint8_t port, char *buf, int maxSpeed) {
	set_tacho_stop_action_inx( 	port, TACHO_HOLD );

	if (strcmp("lift", buf) == 0) {
		set_tacho_speed_sp( port, maxSpeed * (25)); //TESTEN: positiv heben oder senken?
	} else if (strcmp("down", buf) == 0) {
		set_tacho_speed_sp( port, maxSpeed * (-25));//TESTEN: negativ heben oder senken?
	}
	set_tacho_time_sp( 	   port, TIME );
	set_tacho_command_inx( port, TACHO_RUN_TIMED );
	Sleep(PenTIME);
}

void set_motor(uint8_t port, int mode, float speed) {
	set_tacho_stop_action_inx( 	port, mode );
	set_tacho_speed_sp( 		port, (int)speed);
	set_tacho_time_sp( 			port, TIME );
}