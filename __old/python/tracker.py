import math

X_BORDER = 20.0
Y_BORDER = 40.0

PAPER_WIDTH = 29.7
PAPER_HEIGHT = 42.0

X_TOTAL_DISTANCE = PAPER_WIDTH + 2.0 * X_BORDER
Y_DISTANCE_RANGE = (Y_BORDER, Y_BORDER + PAPER_HEIGHT)


def get(data, i):
    return data[(i) % len(data)]


def sum2(data, offset):
    dist = int(len(data) / 2)
    return get(data, offset)[1] + get(data, offset + dist)[1]


def inYRange(data, index):
    val = get(data, index)
    return val >= Y_DISTANCE_RANGE[0] and val <= Y_DISTANCE_RANGE[1]


def calcPosition(data, leftIndex, orthoIndex):
    absPosition = (get(data, leftIndex), get(data, orthoIndex))
    paperPosition = ((absPosition[0] - X_BORDER) / PAPER_WIDTH,
                     (absPosition[1] - Y_BORDER, PAPER_HEIGHT))
    return paperPosition

# data [(angle, distance), (angle, distance), ...]
def evalScan(data):
    # find index
    # print(data)
    value = None
    index = None
    for i in range(len(data)):
        crnt = abs(sum2(data, i) - X_TOTAL_DISTANCE)
        if value == None or crnt < value:
            value = crnt
            index = i

    # print("value: ", value)
    # print("index: ", index)

    orthoIndex = index + len(data) / 4
    alterOrthoIndex = orthoIndex + len(data) / 2

    orthoValid = inYRange(data, orthoIndex)
    alterOrthoValid = inYRange(data, alterOrthoIndex)

    if orthoValid and not alterOrthoValid:
        return calcPosition(data, index, orthoIndex)

    if not orthoValid and alterOrthoValid:
        return calcPosition(data, index + len(data) / 2, alterOrthoIndex)

    return None


class Tracker:
    def __init__(self):
        self._location = (0.0, 0.0)
        self._angle = 0
        self._direction = (1, 0)

    def turn(self, delta):
        self._angle += delta
        rad = math.radians(self._angle)
        self._direction = (math.cos(rad), math.sin(rad))

    def forward(self, forward):
        x = self._location[0] + forward * self._direction[0]
        y = self._location[1] + forward * self._direction[1]
        self._location = (x, y)

    def getLocation(self):
        return self._location


if __name__ == '__main__':
    print('test scan evaluation')
    import random
    testData = [(0.0, )]

    print(testData)
    print(evalScan(testData))

    # print('test class Tracker')
    # tracker = Tracker()
    # tracker.forward(10)
    # print(tracker.getLocation())

    # tracker.turn(90)
    # tracker.forward(20)
    # print(tracker.getLocation())

    # tracker.turn(-180)
    # tracker.forward(20)
    # print(tracker.getLocation())

    # tracker.turn(90)
    # tracker.forward(20)
    # print(tracker.getLocation())

    # tracker.turn(-180)
    # tracker.forward(30)
    # print(tracker.getLocation())
