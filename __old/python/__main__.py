#!/usr/bin/env python3

import signal
import subprocess
import os

from time import sleep

from ev3dev.ev3 import Button
from ev3dev.core import Led
#from ev3dev2.sound import Sound

import batman
#import music

def run():
    global execThread
    execThread = subprocess.Popen("python3 /home/robot/python/batman.py", stdout=subprocess.PIPE, 
                        shell=True, preexec_fn=os.setsid) 

def kill():
    global execThread
    print(execThread, execThread.pid)
    if execThread != None:
        os.killpg(os.getpgid(execThread.pid), signal.SIGTERM)
    execThread = None

execThread = None
btn = Button()
led = Led()

if __name__ == '__main__':
    print('press any button to start drawing')
    led.brightness = 255

    while True:
        if btn.enter:
            print('button pressed')
            if execThread == None:
                led.brightness = 0
                print('start drawing')
                run()
            else:
                led.brightness = 255
                print('abort drawing')
                kill()
            sleep(1)
        else:
            sleep(0.1)
