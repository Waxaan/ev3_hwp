import time

from ev3dev2.motor import MediumMotor, OUTPUT_A, OUTPUT_B, OUTPUT_D, SpeedPercent, MoveTank
from ev3dev2.sensor.lego import UltrasonicSensor, GyroSensor
from ev3dev2.sensor import INPUT_1, INPUT_2

DRIVE_SPEED = 10
TURN_SPEED = 10

PEN_SPEED = 5
PEN_ROTATION = 0.6
SCALE = 200.0


class HardController:
    def __init__(self):
        self._tank = MoveTank(OUTPUT_A, OUTPUT_B)
        self._penMotor = MediumMotor(OUTPUT_D)
        self._usSensor = UltrasonicSensor(INPUT_1)
        self._gyroSensor = GyroSensor(INPUT_2)
        self._gyroSensor.mode = 'GYRO-ANG'
        self.angle = 0

    def up(self):
        self._penMotor.on_for_rotations(
            SpeedPercent(PEN_SPEED), PEN_ROTATION)

    def down(self):
        self._penMotor.on_for_rotations(
            SpeedPercent(-PEN_SPEED), PEN_ROTATION)

    def forward(self, distance):
        try:
            self._tank.on_for_rotations(SpeedPercent(
                DRIVE_SPEED), SpeedPercent(DRIVE_SPEED), distance / SCALE)
        except KeyboardInterrupt:
            self.stop()
            raise

    def turn(self, angle):
        speed = DRIVE_SPEED
        if angle < 0:
            speed *= -1
        self.reset()
        self._tank.on(-speed, speed)
        try:
            while self.getAngle() < abs(angle):
                time.sleep(0.01)
        except KeyboardInterrupt:
            self.stop()
            raise
        self.stop()

    def stop(self):
        self._tank.off()

    def reset(self):
        self.angle = self._gyroSensor.value()

    def getAngle(self):
        return abs(self._gyroSensor.value() - self.angle)

    def getDistance(self):
        return self._usSensor.distance_centimeters


# wird nur bei Ausführung dieses Skriptes ausgeführt, z.B. zum testen
if __name__ == '__main__':
    print('run tests for hardcontroller.py')
    controller = HardController()
    # controller.down()
    controller.up()
    # controller.turn(90)
    # controller.turn(-90)
    # controller.turn(180)
    # controller.forward(10)
    # controller.stop()
    print('done')
