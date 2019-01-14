#include "controller.h"

int void draw(){     
    controller_left(22) 
    controller_forward(85) //linke Schulter
    controller_right(120)
    controller_up()
    controller_forward(10)
    controller_left(180)
    controller_down()
    controller_forward(65) //linker Hals
    controller_left(13)
    controller_forward(85) //linkes Gesicht
    controller_right(8)
    controller_forward(75) //linkes Ohr
    controller_right(160)
    controller_forward(60)
    controller_left(90)
    controller_forward(40) //linker Oberkopf
    controller_right(39)
    controller_forward(40) //rechter Oberkopf
    controller_left(90)
    controller_forward(60) //rechtes Ohr
    controller_right(160)
    controller_forward(75)
    controller_right(8) 
    controller_forward(85) //rechtes Gesicht
    controller_left(13)
    controller_forward(65) //rechter Hals
    controller_left(180)
    controller_up()
    controller_forward(10)
    controller_right(120)
    controller_down()
    controller_forward(85) //rechte Schulter
    controller_up()
    controller_left(180)
    controller_right(23) //direkt zur Maske
    controller_forward(38)
    //alt
    controller_forward(85) 
    //controller_right(60)
    //controller_forward(55)
    controller_left(110)
    controller_down()
    controller_forward(53) //rechte Wange
    controller_right(60)
    controller_forward(42)
    controller_right(60)
    controller_forward(53) //linke Wange
    controller_left(180)
    controller_up()
    controller_forward(20)
    controller_left(150)
    controller_down()
    controller_forward(25) //Maske links senkrecht
    controller_right(115)
    controller_forward(40) //Maske Nase links
    controller_left(50)
    controller_forward(40) //Maske Nase rechts
    controller_right(115)
    controller_forward(30) //Maske rechts senkrecht
    controller_up() //zum Auge rechts
    controller_left(170)
    //controller_forward(25)
    //controller_left(22)
    controller_forward(80)
    controller_left(115)
    controller_down()
    controller_forward(30)
    controller_left(150) // rechtes Innenauge
    controller_forward(20)
    controller_left(70) // rechtes Auge: untere Ecke
    controller_forward(20)
    controller_up()
    controller_left(125)
    controller_forward(22)
    //controller_left(90)
    //controller_forward(45) //Strecke Nase rechts
    //controller_right(85)
    //controller_forward(25)
    //controller_right(50)
    //controller_forward(20)
    //controller_right(85)
    //controller_forward(45)
    //controller_left(100)
    controller_forward(60)
    //controller_left(180)
    //controller_forward(20)
    controller_left(125)
    controller_down()
    controller_forward(20)
    controller_left(70)
    controller_forward(20)
    controller_left(150)
    controller_forward(30)
    controller_down()

    return 0;
}