import tracker

from hardcontroller import HardController


class Controller:
    def __init__(self, scale):
        self._scale = scale
        self._hardController = HardController()
        self._tracker = tracker.Tracker()
        self._isUp = True
        self._drawCount = 0
        self.adjust()

    def stop(self):
        self._hardController.stop()

    def up(self):
        if not self._isUp:
            self._isUp = True
            print('up')
            self._hardController.up()

    def down(self):
        if self._isUp:
            self._isUp = False
            print('down')
            self._hardController.down()

    # TODO: Fehlererkennung
    def forward(self, length):
        print('forward: ', length)
        length /= self._scale
        self._hardController.forward(length)
        self._tracker.forward(length)
        self._drawCount += 1
        if self._drawCount % 3 == 0:
            self.adjust()

    def adjust(self):
        return
        print('adjust...')
        scanResult = self._hardController.getScan()
        location = tracker.evalScan(scanResult)
        virtualLocation = self._tracker.getLocation()

    def left(self, angle):
        print('left: ', angle)
        self._hardController.turn(-angle)
        self._tracker.turn(-angle)

    def right(self, angle):
        print('right: ', angle)
        self._hardController.turn(angle)
        self._tracker.turn(angle)


if __name__ == '__main__':
    print('run tests for controller.py')
    controller = Controller(200.0)
    controller.right(360)
    controller.left(360)
    controller.down()
    controller.up()
    controller.forward(10)
    print('done')
