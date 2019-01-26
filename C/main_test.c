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

//static float *waypoints;

//adding these functions to the scope of the compiler
void set_motor(uint8_t port, int mode, float speed);
void move_pen (uint8_t port, char *buf, int maxSpeed);
void reset_motors(uint8_t port1, uint8_t port2, uint8_t port3);
int parse_file(char *file, int16_t arr[][2],size_t size);
int count_lines(char *file);
void print_array2d(int16_t arr[][2], size_t size);
void convert_coordinates_to_movement(int16_t arr[][2],size_t size);


int main( void )
{
	/*uint8_t Kette, Fahrwerk, Stift;
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
	*/
	size_t size = count_lines("xycoord_v4.txt");

	int16_t data_array[size][2];


	parse_file("xycoord_v4.txt", data_array, size);
	print_array2d(data_array,size);
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

int parse_file(char *file, int16_t arr[][2], size_t size){
	FILE *ptr_file;


	ptr_file = fopen(file, "r");

	if(!ptr_file){
		printf("ERROR! NO FILE CALLED %s FOUND!\n",file);
		return -1;
	}
	printf("OK! FOUND COORDINATES FILE!\n");

	size_t lines = size;
	//printf("number of lines %i\n",lines);

	int16_t coords[lines][2];
    int x, y;
	size_t line = 0;
	printf("PARSING FILE AND WRITING INTO MEMORY!\n");

    while (fscanf(ptr_file,"%i,%i", &x, &y) != EOF) {
		coords[line][0]=x;
		coords[line][1]=y;
		line++;
        //printf("Found coordinates: %i,%i at line %i\n", x, y, line);
    }
	printf("DONE!\n");
	//print_array2d(coords, lines);
	printf("CONVERTING X-Y-PAIRS INTO MOVEMENTS!\n");
	convert_coordinates_to_movement(coords, lines);
	print_array2d(coords, size);


	for(int i = 0; i < size;i++){
		arr[i][0]=coords[i][0];
		arr[i][1]=coords[i][1];
	}

	printf("DONE!\n");
  	fclose(ptr_file);
	return 1;
}

void convert_coordinates_to_movement(int16_t arr[][2], size_t size){

	uint16_t *temp = arr[0];

	for(int i = size; i>0; i--){

		if(arr[i-1][0]==-1){
			arr[i][0]-=arr[i-2][0];
			arr[i][1]-=arr[i-2][1];
			i--;
		}else if(arr[i][0]==-1){

		}else{
			arr[i][0]-=arr[i-1][0];
			arr[i][1]-=arr[i-1][1];
		}
		
	}
	//print_array2d(arr,size);
}

int count_lines(char *file){
	FILE *ptr_file = fopen(file,"r");
	char c;
	size_t count = 1;

	if (!ptr_file)
  		return -1;

	while(!feof(ptr_file)){
		c = fgetc(ptr_file);
		if(c == '\n'){
			count++;
		}
	}
  	fclose(ptr_file);
	return count;
}

void print_array2d(int16_t arr[][2], size_t size){ 
    for (int i = 0; i < size; i++) 
        printf("%i,%i\n", arr[i][0], arr[i][1]); 
} 