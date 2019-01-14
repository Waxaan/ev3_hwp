import signal

try:
    from controller import Controller
    my_turtle = Controller(200.0)
except:
    from turtle import Turtle
    my_turtle = Turtle()


def draw():
    my_turtle.left(22) 
    my_turtle.down()
    my_turtle.forward(85) #linke Schulter
    my_turtle.up()
    my_turtle.right(120)
    my_turtle.forward(10)
    my_turtle.left(180)
    my_turtle.down()
    my_turtle.forward(65) #linker Hals
    my_turtle.left(13)
    my_turtle.forward(85) #linbkes Gesicht
    my_turtle.right(8)
    my_turtle.forward(75) #linkes Ohr
    my_turtle.right(160)
    my_turtle.forward(60)
    my_turtle.left(90)
    my_turtle.forward(50) #linker Oberkopf
    my_turtle.right(39)
    my_turtle.forward(50) #rechter Oberkopf
    my_turtle.left(90)
    my_turtle.forward(60) #rechtes Ohr
    my_turtle.right(160)
    my_turtle.forward(75)
    my_turtle.right(8) 
    my_turtle.forward(85) #rechtes Gesicht
    my_turtle.left(13)
    my_turtle.forward(65) #rechter Hals
    my_turtle.up()
    my_turtle.left(180)
    my_turtle.forward(10)
    my_turtle.right(120)
    my_turtle.down()
    my_turtle.forward(85) #rechte Schulter
    my_turtle.up()
    my_turtle.left(157) #direkt zur Maske
    my_turtle.forward(123)
    my_turtle.left(110)
    my_turtle.down()
    my_turtle.forward(53) #rechte Wange
    my_turtle.right(60)
    my_turtle.forward(42)
    my_turtle.right(60)
    my_turtle.forward(53) #linke Wange
    my_turtle.up()
    my_turtle.left(180)
    my_turtle.forward(20)
    my_turtle.left(150)
    my_turtle.down()
    my_turtle.forward(25) #Maske links senkrecht
    my_turtle.right(115)
    my_turtle.forward(40) #Maske Nase links
    my_turtle.left(50)
    my_turtle.forward(40) #Maske Nase rechts
    my_turtle.right(115)
    my_turtle.forward(30) #Maske rechts senkrecht
    my_turtle.up() #zum Auge rechts
    my_turtle.left(170)
    my_turtle.forward(80)
    my_turtle.left(115)
    my_turtle.down()
    my_turtle.forward(30)
    my_turtle.left(150) # rechtes Innenauge
    my_turtle.forward(20)
    my_turtle.left(70) # rechtes Auge: untere Ecke
    my_turtle.forward(20)
    my_turtle.up()
    my_turtle.left(125)
    my_turtle.forward(22)
    my_turtle.forward(60)
    my_turtle.left(125)
    my_turtle.down()
    my_turtle.forward(20)
    my_turtle.left(70)
    my_turtle.forward(20)
    my_turtle.left(150)
    my_turtle.forward(30)
    my_turtle.up()


def run():
    try:
        draw()
    except KeyboardInterrupt:
        my_turtle.up()
        raise


def exit(signum=None, frame=None):
    print('exit()')
    raise KeyboardInterrupt()


if __name__ == '__main__':
    signal.signal(signal.SIGINT, exit)
    signal.signal(signal.SIGTERM, exit)

    print('start drawing...')
    run()
    print('done')
    input()
