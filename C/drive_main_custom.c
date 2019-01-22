#include <stdio.h>
#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

#include "coroutine.h"
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_sensor.h"
#include "ev3_tacho.h"

#define DRIVE_MOTOR_PORT      OUTPUT_A
#define DRIVE_MOTOR_EXT_PORT  EXT_PORT__NONE_
#define CHAIN_MOTOR_PORT      OUTPUT_C
#define CHAIN_MOTOR_EXT_PORT  EXT_PORT__NONE_
#define PEN_MOTOR_PORT        OUTPUT_C
#define PEN_MOTOR_EXT_PORT    EXT_PORT__NONE_

int max_speed;
int app_alive;
int moving;   /* Current moving */
int command;  /* Command for the 'drive' coroutine */

enum {
	Drive, Chain, Pen
};

enum {
	MOVE_NONE,
	MOVE_FORWARD,
	STEP_BACKWARD,
};

uint8_t motor[4] = { DESC_LIMIT, DESC_LIMIT, DESC_LIMIT, DESC_LIMIT };



static void _run_forever( int l_speed, int r_speed )
{
	set_tacho_speed_sp( motor[Drive], l_speed );
	set_tacho_speed_sp( motor[Chain], r_speed );
	set_tacho_speed_sp( motor[Pen], r_speed );
	multi_set_tacho_command_inx( motor, TACHO_RUN_FOREVER );
}

static void _run_timed( int l_speed, int r_speed, int ms )
{
	set_tacho_speed_sp( motor[Drive], l_speed );
	set_tacho_speed_sp( motor[Chain], r_speed );
	set_tacho_speed_sp( motor[Pen], r_speed );
	multi_set_tacho_time_sp( motor, ms );
	multi_set_tacho_command_inx( motor, TACHO_RUN_TIMED );
}

static void _stop( void )
{
	multi_set_tacho_command_inx( motor, TACHO_STOP );
}

int app_init( void )
{
	char s[16];

	if ( ev3_search_tacho_plugged_in( DRIVE_MOTOR_PORT, DRIVE_MOTOR_EXT_PORT, motor + Drive, 0 )) {
		get_tacho_max_speed( motor[Drive], &max_speed );
		/* Reset the motor */
		set_tacho_command_inx( motor[Drive], TACHO_RESET );
	} else {
		printf( "DRIVE motor (%s) is NOT found.\n", ev3_port_name( DRIVE_MOTOR_PORT, DRIVE_MOTOR_EXT_PORT, 0, s ));
		/* Inoperative without left motor */
		return ( 0 );
	}
	if ( ev3_search_tacho_plugged_in( CHAIN_MOTOR_PORT, CHAIN_MOTOR_EXT_PORT, motor + Chain, 0 )) {
		/* Reset the motor */
		set_tacho_command_inx( motor[Chain], TACHO_RESET );
	} else {
		printf( "CHAIN motor (%s) is NOT found.\n", ev3_port_name( CHAIN_MOTOR_PORT, CHAIN_MOTOR_EXT_PORT, 0, s ));
		/* Inoperative without right motor */
		return ( 0 );
	}
	if ( ev3_search_tacho_plugged_in( PEN_MOTOR_PORT, PEN_MOTOR_EXT_PORT, motor + Pen, 0 )) {
		/* Reset the motor */
		set_tacho_command_inx( motor[Chain], TACHO_RESET );
	} else {
		printf( "PEN motor (%s) is NOT found.\n", ev3_port_name( PEN_MOTOR_PORT, PEN_MOTOR_EXT_PORT, 0, s ));
		/* Inoperative without right motor */
		return ( 0 );
	}
	command	= moving = MOVE_NONE;

	printf( "Press BACK on the EV3 brick for EXIT...\n" );
	return ( 1 );
}


CORO_CONTEXT( handle_brick_control );
CORO_CONTEXT( drive );

/* Coroutine of the EV3 brick keys handling */
CORO_DEFINE( handle_brick_control )
{
	CORO_LOCAL uint8_t keys, pressed = EV3_KEY__NONE_;

	CORO_BEGIN();
	for ( ; ; ) {
		/* Waiting any key is pressed or released */
		CORO_WAIT( ev3_read_keys( &keys ) && ( keys != pressed ));
		pressed = keys;

		if ( pressed & EV3_KEY_BACK ) {
			/* Stop the vehicle */
			command = MOVE_NONE;
			/* Quit */
			app_alive = 0;

		} else if ( pressed & EV3_KEY_UP ) {
			/* Stop the vehicle */
			command = MOVE_NONE;
			/* Switch mode */
			_set_mode(( mode == MODE_REMOTE ) ? MODE_AUTO : MODE_REMOTE );
		}
		CORO_YIELD();
	}
	CORO_END();
}


/* Coroutine of control the motors */
CORO_DEFINE( drive )
{
	CORO_LOCAL int speed_linear, speed_circular;
	CORO_LOCAL int _wait_stopped;

	CORO_BEGIN();
	speed_linear = max_speed * SPEED_LINEAR / 100;
	speed_circular = max_speed * SPEED_CIRCULAR / 100;

	for ( ; ; ) {
		/* Waiting new command */
		CORO_WAIT( moving != command );

		_wait_stopped = 0;
		switch ( command ) {

		case MOVE_NONE:
			_stop();
			_wait_stopped = 1;
			break;

		case MOVE_FORWARD:
			_run_forever( -speed_linear, -speed_linear );
			break;

		case STEP_BACKWARD:
			_run_timed( speed_linear, speed_linear, 1000 );
			_wait_stopped = 1;
			break;
		}
		moving = command;

		if ( _wait_stopped ) {
			/* Waiting the command is completed */
			CORO_WAIT( !_is_running());

			command = moving = MOVE_NONE;
		}
	}
	CORO_END();
}


int main( void )
{
	printf( "Waiting the EV3 brick online...\n" );
	if ( ev3_init() < 1 ) return ( 1 );

	printf( "*** ( EV3 ) Hello! ***\n" );
	ev3_sensor_init();
	ev3_tacho_init();

	app_alive = app_init();
	while ( app_alive ) {
		command = MOVE_FORWARD;

		CORO_CALL(handle_brick_control);
		CORO_CALL(drive);
		Sleep(1000);

		command = STEP_BACKWARD;

		CORO_CALL(handle_brick_control);
		CORO_CALL(drive);
		Sleep(1000)
	}
	ev3_uninit();
	printf( "*** ( EV3 ) Bye! ***\n" );

	return ( 0 );
}