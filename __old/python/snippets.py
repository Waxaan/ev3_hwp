#from ev3dev2.motor import LargeMotor, OUTPUT_A, OUTPUT_B, SpeedPercent
#
#m = LargeMotor(OUTPUT_A)
#m.on_for_rotations(SpeedPercent(75), 5)

# from ev3dev2.sensor import INPUT_1
# from ev3dev2.sensor.lego import TouchSensor
# from ev3dev2.led import Leds

# ts = TouchSensor()
# leds = Leds()

# print("Press the touch sensor to change the LED color!")

# while True:
#     if ts.is_pressed:
#         leds.set_color("LEFT", "GREEN")
#         leds.set_color("RIGHT", "GREEN")
#     else:
#         leds.set_color("LEFT", "RED")
#         leds.set_color("RIGHT", "RED")

# from ev3dev2.sensor import INPUT_1
# from ev3dev2.sensor.lego import ColorSensor

# colorSensor = ColorSensor()

# oldColor = -1

# while True:
#     if oldColor != colorSensor.color:
#         oldColor = colorSensor.color
#         print(colorSensor.color_name)


# from ev3dev2.sensor import INPUT_2
# from ev3dev2.sensor.lego import UltrasonicSensor

# usSensor = UltrasonicSensor()

# while True:
#     print(usSensor.distance_centimeters)
#     input()

# from ev3dev2.sensor import INPUT_1
# from ev3dev2.sensor.lego import GyroSensor


# gyroSensor = GyroSensor()

# while True:
#     print(gyroSensor.rate_and_angle)
#     input()


# from ev3dev2.motor import MediumMotor, OUTPUT_A, SpeedPercent

# motor = MediumMotor()

# motor.on_for_rotations(SpeedPercent(75), 5)

from ev3dev2.motor import LargeMotor, MediumMotor, OUTPUT_A, OUTPUT_B, OUTPUT_C, SpeedPercent, MoveTank
from ev3dev2.sensor.lego import UltrasonicSensor, GyroSensor
from ev3dev2.sensor import INPUT_1, INPUT_2

tank_drive = MoveTank(OUTPUT_A, OUTPUT_B)
motor = MediumMotor(OUTPUT_C)

print('type in command')

usSensor = UltrasonicSensor(INPUT_1)
gyroSensor = GyroSensor(INPUT_2)

while True:
    i = input()
    args = i.split(' ')
    if len(args) > 1:
        i = args[0]
        arg = float(args[1])
    if i == 'up':
        print('up')
        motor.on_for_rotations(SpeedPercent(5), arg)
    if i == 'down':
        print('down')
        motor.on_for_rotations(SpeedPercent(-5), arg)
    if i == 'drive':
        tank_drive.on_for_seconds(SpeedPercent(10), SpeedPercent(10), arg)
    if args[0] == 'turn':
        speed = 10
        if arg < 0: 
            arg *= -1
            speed *= -1
        tank_drive.on_for_seconds(SpeedPercent(speed), SpeedPercent(-speed), arg)
    if i == 'distance':
        print(usSensor.distance_centimeters)
    if i == 'gyro':
        print(gyroSensor.rate_and_angle)
    if i == 'q':
        break