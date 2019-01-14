#include "controller.h"

#define SCALE 200

int controller_init(Controller &controller, int scale)
{
    controller.scale = scale;
    controller.hardController = hardcontroller_init();
    controller.isUp = True;
    controller.drawCount = 0;
    controller.exit = False;
    return 0;
}

int controller_up(int var){

    return 0;
}

int controller_down(int var){

    return 0;
}

int controller_left(int)
{

}
int controller_right(int)
{

}

int controller_forward(int){

}