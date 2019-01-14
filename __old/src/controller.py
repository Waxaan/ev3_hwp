import time

from hardcontroller import HardController
from tracker import Tracker


class Controller:
    def __init__(self):
        self._hardController = HardController()
        self._tracker = Tracker()
        #self.locate()
        self.isUp = True

    def up(self):
        if not self.isUp:
            self.isUp = True
            print('up')
            self._hardController.up()

    def down(self):
        if self.isUp:
            self.isUp = False
            print('down')
            self._hardController.down()

    def goto(self, location):
        pass
        #TODO

    # TODO: Fehlererkennung
    def forward(self, lengthCM):
        print('forward: ', lengthCM)
        self._hardController.forward(lengthCM)

    # def locate(self):
    #     distances = []
    #     for i in range(36):
    #         self._hardController.turn(10)
    #         distances.append(self._hardController.getDistance())
    #         print(distances)
    #     self._hardController.turn(10)
    #     self._tracker.evalScan(distances)

    def left(self, angle):
        print('left: ', angle)
        self._hardController.turn(-angle)
        

    def right(self, angle):
        print('right: ', angle)
        self._hardController.turn(angle)
        

if __name__ == '__main__':
    print('run tests for controller.py')
    controller = Controller()
    controller.right(360)
    controller.left(360)
    controller.down()
    controller.up()
    controller.forward(10)
    print('done')
