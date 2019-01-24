#include <stdio.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )
#include <inttypes.h>

int main( void )
{
	int DATALEN = 4;
	int TIME = 1000;
	int PenTIME = 250;
	uint8_t Kette; 
	uint8_t Fahrwerk; 
	uint8_t Stift;
	uint8_t sn;
	int maxKette;
	int maxFahrwerk;
	int maxStift;
	char s[ 256 ];

	if ( ev3_init() == -1 ) return ( 1 );
	while ( ev3_tacho_init() < 1 ) Sleep( 1000 );

	printf( "*** ( EV3 ) Hello! ***\n" );

	printf( "Found tacho motors:\n" );
	for (int i = 0; i < DESC_LIMIT; i++ ) {
		if ( ev3_tacho[ i ].type_inx != TACHO_TYPE__NONE_ ) {
			printf( "  type = %s\n", ev3_tacho_type( ev3_tacho[ i ].type_inx ));
			printf( "  port = %s\n", ev3_port_port_name( i, s ));
		}
	}

	//setze die Ports und Geschindigkeit der Kette, Fahrwerk und des Stifts
	if (ev3_search_tacho( LEGO_EV3_L_MOTOR, &Kette,	0)) {
		get_tacho_max_speed( Kette, &maxKette);
		maxKette /= 50;
		printf("Kette gefunden\n");
	} else { printf("Kette nicht gefunden\n");	return(1);	}

	if (ev3_search_tacho( LEGO_EV3_L_MOTOR, &Fahrwerk,	1)) {
		get_tacho_max_speed( Fahrwerk, &maxFahrwerk);
		maxFahrwerk /= 50;
		printf("Fahrwerk gefunden\n");
	} else { printf("Fahrwerk nicht gefunden\n");	return(1);	}

	if (ev3_search_tacho( LEGO_EV3_M_MOTOR, &Stift,	0)) {
		get_tacho_max_speed( Stift, &maxStift);
		maxStift /= 50;
		printf("Stift gefunden\n");
	} else { printf("Stift nicht gefunden\n"); 	return(1);	}

	 float data[4][3] = {{10.0, 10.0, 1.0}, {5.0, 20.0, 1.0}, {-10.0, 3.0, 0.0}, {20.0, -10.0, 0.0}};
	
	//hebe Stift zu beginn des Laufs
	set_tacho_stop_action_inx( 	Stift, TACHO_HOLD );
	set_tacho_speed_sp( 		Stift, maxStift * (-25));
	set_tacho_time_sp( 			Stift, TIME );
	set_tacho_command_inx( 		Stift, TACHO_RUN_TIMED );
	Sleep(PenTIME);
	
	for(int i = 0; i < 4; i++) {
		//Stift senken, falls aufgesetzt werden soll
		if(data[i][3] != 0.0) {
			set_tacho_stop_action_inx( 	Stift, TACHO_HOLD );
			set_tacho_speed_sp( 		Stift, maxStift * 25 );
			set_tacho_time_sp( 			Stift, PenTIME );
			set_tacho_command_inx( 		Stift, TACHO_RUN_TIMED );
		}
		Sleep(PenTIME);

		//setze Eigenschaften des Ketten-Motors
		set_tacho_stop_action_inx( 	Kette, TACHO_HOLD );
		set_tacho_speed_sp( 		Kette, maxKette * data[i][0] );
		set_tacho_time_sp( 			Kette, TIME );

		//setze Eigenschaften des Fahrwerks
		set_tacho_stop_action_inx( 	Fahrwerk, TACHO_HOLD );
		set_tacho_speed_sp( 		Fahrwerk, maxFahrwerk * data[i][1] );
		set_tacho_time_sp( 			Fahrwerk, TIME );
		
		//Lasse Kette + Fahrwerk fahren
		set_tacho_command_inx( Kette,	TACHO_RUN_TIMED );
		set_tacho_command_inx( Fahrwerk,TACHO_RUN_TIMED );

		Sleep(TIME);

		if(data[i][3] != 0.0) {
			set_tacho_stop_action_inx( 	Stift, TACHO_HOLD );
			set_tacho_speed_sp( 		Stift, maxStift * (-25));
			set_tacho_time_sp( 			Stift, PenTIME );
			set_tacho_command_inx( 		Stift, TACHO_RUN_TIMED );
		}
		Sleep(PenTIME);
	}
 
	ev3_uninit();
	printf( "*** ( EV3 ) Bye! ***\n" );

	return ( 0 );
}