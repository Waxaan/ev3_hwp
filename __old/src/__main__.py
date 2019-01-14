#!/usr/bin/env python3

import batman
from ev3dev.ev3 import Button
from time import sleep

btn = Button()

def runThreaded():
    batman.draw()

thread = None

if __name__ == '__main__':
    print('press any button to start drawing')
    while True:
        if btn.any():
            if thread == None:
                print('start drawing')
                runThreaded()
        else:
            sleep(0.1)