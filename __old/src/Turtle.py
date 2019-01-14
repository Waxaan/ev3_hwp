def draw():
    try:
        from controller import Controller
        my_turtle = Controller()
    except:
        from turtle import Turtle
        my_turtle = Turtle()
        
    my_turtle.left(22) 
    my_turtle.forward(85) #linke Schulter
    my_turtle.right(120)
    my_turtle.up()
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
    my_turtle.forward(40) #linker Oberkopf
    my_turtle.right(39)
    my_turtle.forward(40) #rechter Oberkopf
    my_turtle.left(90)
    my_turtle.forward(60) #rechtes Ohr
    my_turtle.right(160)
    my_turtle.forward(75)
    my_turtle.right(8) 
    my_turtle.forward(85) #rechtes Gesicht
    my_turtle.left(13)
    my_turtle.forward(65) #rechter Hals
    my_turtle.left(180)
    my_turtle.up()
    my_turtle.forward(10)
    my_turtle.right(120)
    my_turtle.down()
    my_turtle.forward(85) #rechte Schulter
    my_turtle.up()
    my_turtle.left(180)
    my_turtle.right(23) #direkt zur Maske
    my_turtle.forward(38)
    #alt
    my_turtle.forward(85) 
    #my_turtle.right(60)
    #my_turtle.forward(55)
    my_turtle.left(110)
    my_turtle.down()
    my_turtle.forward(53) #rechte Wange
    my_turtle.right(60)
    my_turtle.forward(42)
    my_turtle.right(60)
    my_turtle.forward(53) #linke Wange
    my_turtle.left(180)
    my_turtle.up()
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
    #my_turtle.forward(25)
    #my_turtle.left(22)
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
    #my_turtle.left(90)
    #my_turtle.forward(45) #Strecke Nase rechts
    #my_turtle.right(85)
    #my_turtle.forward(25)
    #my_turtle.right(50)
    #my_turtle.forward(20)
    #my_turtle.right(85)
    #my_turtle.forward(45)
    #my_turtle.left(100)
    my_turtle.forward(60)
    #my_turtle.left(180)
    #my_turtle.forward(20)
    my_turtle.left(125)
    my_turtle.down()
    my_turtle.forward(20)
    my_turtle.left(70)
    my_turtle.forward(20)
    my_turtle.left(150)
    my_turtle.forward(30)


if __name__ == '__main__':
    draw()
    input()