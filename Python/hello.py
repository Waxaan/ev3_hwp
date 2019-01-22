#!/usr/bin/env python3
from ev3dev2.motor import LargeMotor, OUTPUT_A, OUTPUT_B, SpeedPercent, MoveTank
from ev3dev2.sensor import INPUT_1
from ev3dev2.sensor.lego import TouchSensor
from ev3dev2.led import Leds

import time


def main():
    print('Hello World!')
    m = LargeMotor(OUTPUT_A)
    m.on_for_rotations(SpeedPercent(75), 5)
    time.sleep(5)


if __name__ == '__main__':
    main()
