#include <stdio.h>
#include <string.h>
#include <math.h>

#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include <unistd.h>

#define Sleep(msec) usleep((msec)*1000)
#define WHLDIAMETER 43.2
#define WHLCIRC (M_PI * WHLDIAMETER)
#define TRCKDIAMETER 40.0
#define TRCKCIRC (M_PI * TRCKDIAMETER)

static uint16_t const DINA4_X = 210;
static uint16_t const DINA4_Y = 297;

//static float *waypoints;

//adding these functions to the scope of the compiler
void set_motor(uint8_t port, int mode, float speed, int16_t pos);
void move_pen(uint8_t port, char *buf, int maxSpeed);
void reset_motors(uint8_t port1, uint8_t port2, uint8_t port3);
int is_running(uint8_t port);
int calibrate_pen(uint8_t port, int speed);
int calibrate_track(uint8_t port, int speed);
void set_kette(uint8_t port, int mode, float speed, int16_t pos);
void set_fahrwerk(uint8_t port, int mode, float speed, int16_t pos);

int parse_file(char *file, int16_t arr[][2], size_t size);
int count_lines(char *file);
void print_array2d(int16_t arr[][2], size_t size);
void convert_coordinates_to_movement(int16_t arr[][2], size_t size);
void scale_coordinates(int16_t arr[][2], size_t size, uint16_t max_x, uint16_t max_y);

int main(void)
{
	//TACHO_RUNNING = 1
	//TACHO_RAMPING = 2
	//TACHO_HOLDING = 4
	//TACHO_OVERLOADED = 8
	//TACHO_STALLED = 16

	// USE BITWISE AND OPERATOR (&) TO CHECK FLAGS

	size_t size = count_lines("xycoord_v5.txt");

	int16_t data_array[size][2];

	parse_file("xycoord_v5.txt", data_array, size);

	printf("Scaling coordinates to fit into DinA4!\n");

	scale_coordinates(data_array, size, DINA4_X, DINA4_Y);

	int stiftUp = 1;
	uint8_t Kette, Fahrwerk, Stift;
	int maxKette, maxFahrwerk, maxStift;
	char s[256];

	if (ev3_init() == -1)
		return (1);
	while (ev3_tacho_init() < 1)
		Sleep(1000);

	printf("*** ( EV3 ) Hello! ***\n");

	//setze die Ports und Geschindigkeit der Kette, Fahrwerk und des Stifts

	if( (ev3_search_tacho( LEGO_EV3_L_MOTOR, &Fahrwerk,    0))
	 &&	(ev3_search_tacho( LEGO_EV3_L_MOTOR, &Kette,	1)) 
	 && (ev3_search_tacho( LEGO_EV3_M_MOTOR, &Stift,    0))	)
	{
		//printf("KETTE: %i\nFAHRWERK: %i\nSTIFT: %i\n",Kette, Fahrwerk, Stift);

		printf("Alles gefunden!\n");

		get_tacho_max_speed(Kette, &maxKette);
		get_tacho_max_speed(Fahrwerk, &maxFahrwerk);
		get_tacho_max_speed(Stift, &maxStift);
		maxKette /= 45;
		maxFahrwerk /= 15;
		maxStift /= 3;
	}
	else
	{
		printf("Mindestens einen Motor nicht gefunden!\n");
		return (1);
	}

	printf("Kalibriere Stift!\n");
	calibrate_pen(Stift, maxStift);
	printf("Done!\n");
	set_tacho_command_inx(Kette, TACHO_STOP);
	printf("Kalibriere Kette\n!");
	calibrate_track(Kette, maxKette);
	printf("Done!\n");

	reset_motors(Kette, Fahrwerk, Stift);

	for(int i = 0; i < size; i++) {
		if(data_array[i][0] == -1) {
			move_pen(Stift, "lift", maxStift);
			stiftUp = 1;
			printf("Lifting Pen!\n");

		}else{


			//if(data_array[i][1]>100) //ADjust speed here maybe
			//if(data_array[i][1]>50)
			//if(data_array[i][1]>10)
			//if(data_array[i][1]>5)
			//if(data_array[i][1]>1)
			//set_motor(Kette, TACHO_HOLD, maxFahrwerk , data_array[i][0]);					
			//set_motor(Fahrwerk, TACHO_HOLD, maxFahrwerk , data_array[i][1]);	

			set_kette(Kette, TACHO_HOLD, maxFahrwerk , data_array[i][0]);
			set_fahrwerk(Fahrwerk, TACHO_HOLD, maxFahrwerk , data_array[i][1]);

			set_tacho_command_inx(Kette, TACHO_RUN_TO_ABS_POS);
			set_tacho_command_inx(Fahrwerk, TACHO_RUN_TO_ABS_POS);

			Sleep(50); //give time to get running
			while(is_running(Fahrwerk)==1||is_running(Kette)==1){ //pause while moving

				FLAGS_T state = TACHO_STATE__NONE_;
				get_tacho_state_flags(Kette, &state); //checking if reached maximum/minimum
				//printf("State of Kette %i: \n",state);
			}
			Sleep(1000);

			if(stiftUp==1){
				printf("Lowering Pen!\n");
				move_pen(Stift, "down", maxStift);
				stiftUp=0;
			}
		}
	}


	printf("Lifting Pen!\n");
	move_pen(Stift, "lift", maxStift);
	
	reset_motors(Kette, Fahrwerk, Stift);
	ev3_uninit();
	printf("*** ( EV3 ) Bye! ***\n");
	return (0);
}

void move_pen(uint8_t port, char *buf, int maxSpeed)
{
	set_tacho_stop_action_inx(port, TACHO_HOLD);

	if (strcmp("lift", buf) == 0)
	{
		set_tacho_speed_sp(port, maxSpeed);
		set_tacho_position_sp(port, -400); // (Ausprobieren welcher DEGREEinput gut)
	}
	else if (strcmp("down", buf) == 0)
	{
		set_tacho_speed_sp(port, maxSpeed);
		set_tacho_position_sp(port, 400); // (Ausprobieren welcher DEGREEinput gut)
	}

	set_tacho_command_inx(port, TACHO_RUN_TO_REL_POS);
	Sleep(50); //give time to start running
	while (is_running(port) == 1)
	{
		//pause
	}

	set_tacho_command_inx(port, TACHO_RESET);
}

void set_kette(uint8_t port, int mode, float speed, int16_t pos)
{

	set_tacho_stop_action_inx(port, mode);
	set_tacho_speed_sp(port, (int)speed);
	//printf("port: %i\n",port);

	double deg = 1 / (TRCKCIRC / (360 * pos)); // CIRC * #deg = distance traveled
	double deg_m = 0 - deg;
	printf("Travelling to X: %i(mm) ", pos);
	set_tacho_position_sp(port, (int)deg_m);
}

void set_fahrwerk(uint8_t port, int mode, float speed, int16_t pos)
{

	set_tacho_stop_action_inx(port, mode);
	set_tacho_speed_sp(port, (int)speed);
	//printf("port: %i\n",port);
	double deg = 1 / (WHLCIRC / (360 * pos)); // CIRC * #deg = distance traveled
	printf("and Y: %i(mm).\n", pos);
	//printf("I will need %f degrees rotation for that. That equals %f rotations.\n", deg, deg/360); FOR RELATIVE MOVEMENTS ONLY
	set_tacho_position_sp(port, (int)deg);
}

void reset_motors(uint8_t port1, uint8_t port2, uint8_t port3)
{
	set_tacho_command_inx(port1, TACHO_RESET);
	set_tacho_command_inx(port2, TACHO_RESET);
	set_tacho_command_inx(port3, TACHO_RESET);
}

int calibrate_pen(uint8_t port, int speed) // TODO: FIX
{
	set_tacho_stop_action_inx(port, TACHO_HOLD);
	set_tacho_speed_sp(port, speed);
	FLAGS_T state = TACHO_STATE__NONE_;

	set_tacho_position_sp(port, 650);

	set_tacho_command_inx(port, TACHO_RUN_TO_REL_POS);

	Sleep(3000);
	set_tacho_command_inx(port, TACHO_RESET);

	printf("HIT BOTTOM WITH PEN! LIFTING AGAIN!");

	set_tacho_speed_sp(port, speed);
	set_tacho_position_sp(port, -500); // (Ausprobieren welcher DEGREEinput gut)
	set_tacho_command_inx(port, TACHO_RUN_TO_REL_POS);
	Sleep(3000);
}

int calibrate_track(uint8_t port, int speed)
{
	set_tacho_stop_action_inx(port, TACHO_HOLD);
	set_tacho_speed_sp(port, speed * 5);

	set_tacho_position_sp(port, 300);
	set_tacho_command_inx(port, TACHO_RUN_TO_REL_POS);

	Sleep(4000);

	FLAGS_T state = TACHO_STATE__NONE_;

	set_tacho_polarity(port, "inversed");
	set_tacho_command_inx(port, TACHO_RUN_FOREVER);

	while (!(state & TACHO_STALLED))
	{
		get_tacho_state_flags(port, &state);
		//printf("current state: %i\n", state);
	}

	printf("HIT THE RIGHT BORDER\n");

	set_tacho_command_inx(port, TACHO_STOP);
	set_tacho_polarity(port, "normal");
	set_tacho_position_sp(port, -55);
	set_tacho_command_inx(port, TACHO_RUN_TO_REL_POS);
	Sleep(2000);

	//int pos = 0;
	//get_tacho_position_sp(port, pos);
	//printf("pos: %i", pos);
}

int is_running(uint8_t port)
{ //return 0 if isnt running 1 if is running
	FLAGS_T state = TACHO_STATE__NONE_;

	get_tacho_state_flags(port, &state);

	/*if (state & TACHO_HOLDING)
	{
		printf("TACHO_HOLDING\n");
	}
	if (state & TACHO_RAMPING)
	{
		printf("TACHO_RAMPING\n");
	}
	if (state & TACHO_RUNNING)
	{
		printf("TACHO_RUNNING\n");
	}
	if (state & TACHO_OVERLOADED)
	{
		printf("TACHO_OVERLOADED\n");
	}
	if (state & TACHO_STALLED)
	{
		printf("TACHO_STALLED\n");
	}*/

	if (state == TACHO_HOLDING || state == TACHO_STATE__NONE_)
		return 0;
	if (state == TACHO_RAMPING || state == TACHO_RUNNING || state == TACHO_STALLED)
		return 1;
	return 0;
}

int parse_file(char *file, int16_t arr[][2], size_t size)
{
	FILE *ptr_file;

	ptr_file = fopen(file, "r");

	if (!ptr_file)
	{
		printf("ERROR! NO FILE CALLED %s FOUND!\n", file);
		return -1;
	}
	printf("OK! FOUND COORDINATES FILE!\n");

	size_t lines = size;
	//printf("number of lines %i\n",lines);

	int16_t coords[lines][2];
	int x, y;
	size_t line = 0;
	printf("PARSING FILE AND WRITING INTO MEMORY!\n");

	while (fscanf(ptr_file, "%i,%i", &x, &y) != EOF)
	{
		coords[line][0] = x;
		coords[line][1] = y;
		line++;
		//printf("Found coordinates: %i,%i at line %i\n", x, y, line);
	}
	printf("DONE!\n");

	//print_array2d(coords, lines);
	//printf("SCALING COORDINATES TO FIT INTO DinA4 BOUNDARIES - IF NECESSARY!\n");
	//scale_coordinates(coords, lines, DINA4_X, DINA4_Y);
	//printf("CONVERTING X-Y-PAIRS INTO MOVEMENTS!\n");
	//convert_coordinates_to_movement(coords, lines);
	//print_array2d(coords, size);

	for (int i = 0; i < size; i++)
	{
		arr[i][0] = coords[i][0];
		arr[i][1] = coords[i][1];
	}

	//printf("DONE!\n");
	fclose(ptr_file);
	return 1;
}

void convert_coordinates_to_movement(int16_t arr[][2], size_t size) //deprecated
{

	uint16_t *temp = arr[0];

	for (int i = size; i > 0; i--)
	{

		if (arr[i - 1][0] == -1)
		{
			arr[i][0] -= arr[i - 2][0];
			arr[i][1] -= arr[i - 2][1];
			i--;
		}
		else if (arr[i][0] == -1)
		{
		}
		else
		{
			arr[i][0] -= arr[i - 1][0];
			arr[i][1] -= arr[i - 1][1];
		}
	}
}

void scale_coordinates(int16_t arr[][2], size_t size, uint16_t max_x, uint16_t max_y)
{

	double highest_x = 0, highest_y = 0;
	double scale_factor = 1;

	for (int i = 0; i < size; i++)
	{
		if (arr[i][0] > highest_x)
			highest_x = arr[i][0];
		if (arr[i][1] > highest_y)
			highest_y = arr[i][1];
	}

	printf("Highest values are:\nX: %lf (Allowed: X: %i)\nY: %lf (Allowed: Y: %i)\n", highest_x, max_x, highest_y, max_y);

	if (highest_x < max_x && highest_y < max_y)
	{ //if there is no coordinate that surpasses the boundary
		printf("Already in Bounds!\n");
		return;
	}

	double factor_x = highest_x / max_x, factor_y = highest_y / max_y;
	printf("factor_x: %lf\n", factor_x);
	printf("factor_y: %lf\n", factor_y);

	if (factor_x > factor_y)
	{ //if the x coordinates pass the boundary more than the y
		printf("X ratio with %lf bigger than Y ratio with %lf!\n", highest_x, highest_y);
		scale_factor = max_x / highest_x;
	}

	if (factor_y >= factor_x)
	{ //if the y coordinates pass the boundary more than the x
		printf("Y ratio with %lf bigger than X ratio with %lf!\n", highest_y, highest_x);
		scale_factor = max_y / highest_y;
	}

	int16_t new_x, new_y;

	for (int i = 0; i < size; i++)
	{
		if (arr[i][0] != -1)
		{
			new_x = (int16_t)(arr[i][0] * scale_factor);
			new_y = (int16_t)(arr[i][1] * scale_factor);
			arr[i][0] = new_x;
			arr[i][1] = new_y;
		}
	}
}

int count_lines(char *file)
{
	FILE *ptr_file = fopen(file, "r");
	char c;
	size_t count = 1;

	if (!ptr_file)
		return -1;

	while (!feof(ptr_file))
	{
		c = fgetc(ptr_file);
		if (c == '\n')
		{
			count++;
		}
	}
	fclose(ptr_file);
	return count;
}

void print_array2d(int16_t arr[][2], size_t size)
{
	for (int i = 0; i < size; i++)
		printf("%i,%i\n", arr[i][0], arr[i][1]);
}

void set_motor(uint8_t port, int mode, float speed, int16_t pos)
{
	set_tacho_stop_action_inx(port, mode);
	set_tacho_speed_sp(port, (int)speed);
	//printf("port: %i\n",port);

	if (port == 1)
	{											   //Kette
		double deg = 1 / (TRCKCIRC / (360 * pos)); // CIRC * #deg = distance traveled
		printf("Travelling to X: %i(mm) ", pos);
		set_tacho_position_sp(port, (int)deg);
	}

	/*if(port==0){ //Fahrwerk
		double deg = 1 / (WHLCIRC / (360 * pos)); // CIRC * #deg = distance traveled
		printf("	and Y: %i(mm).\n", pos);
		//printf("I will need %f degrees rotation for that. That equals %f rotations.\n", deg, deg/360); FOR RELATIVE MOVEMENTS ONLY
		set_tacho_position_sp(port, (int)deg);
	}*/
}