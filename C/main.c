#include <stdio.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

int main(void) {

	int i;
	uint8_t sn;
	FLAGS_T state;
	char s[256];

	//wait for the brick to initialize
	if ( ev3_init() == -1 ) return ( 1 );	
	while ( ev3_tacho_init() < 1 ) Sleep( 1000 );

	//init successful
	printf( "*** ( EV3 ) Hello! ***\n" );

	if ( ev3_search_tacho( LEGO_EV3_M_MOTOR, &sn, 0 )) {
		int max_speed;

		printf( "LEGO_EV3_M_MOTOR is found, run for 5 sec...\n" );
		get_tacho_max_speed( sn, &max_speed );
		printf("  max_speed = %d\n", max_speed );
		set_tacho_stop_action_inx( sn, TACHO_COAST );
		set_tacho_speed_sp( sn, max_speed * 2 / 3 );
		set_tacho_time_sp( sn, 5000 );
		set_tacho_ramp_up_sp( sn, 2000 );
		set_tacho_ramp_down_sp( sn, 2000 );
		set_tacho_command_inx( sn, TACHO_RUN_TIMED );
		/* Wait tacho stop */
		Sleep( 100 );
		do {
			get_tacho_state_flags( sn, &state );
		} while ( state );
		printf( "run to relative position...\n" );
		set_tacho_speed_sp( sn, max_speed / 2 );
		set_tacho_ramp_up_sp( sn, 0 );
		set_tacho_ramp_down_sp( sn, 0 );
		set_tacho_position_sp( sn, 90 );
		for ( i = 0; i < 8; i++ ) {
			set_tacho_command_inx( sn, TACHO_RUN_TO_REL_POS );
			Sleep( 500 );
		}

		ev3_uninit();
		printf( "*** ( EV3 ) Bye! ***\n" );
	return 0;
}