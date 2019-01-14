X_TOTAL_DISTANCE = 29.7 + 2 * 20
Y_DISTANCE_RANGE = (40, 40 + 42)

def sum2(data, offset):
    dist = int(len(data) / 2)
    return data[offset] + data[(offset + dist) % len(data)]


def validate(data, value):
    pass

class Tracker:
    def __init__(self):
        self.location = None
        self.angle = None

    def evalScan(self, data):
        # find offset that produces lowest sum3
        print("data: ", data)
        value = None
        index = None
        for i in range(len(data)):
            crnt = abs(sum2(data, i) - X_TOTAL_DISTANCE)
            if value == None or crnt < value:
                value = crnt
                index = i
        
        print("value: ", value)
        print("index: ", index)
        

if __name__ == '__main__':
    tracker = Tracker()

    testData = [255, 250, 255, 3, 255, 250, 255, 250, 255,
                100, 80, 70, 60,    20,   60, 65, 70, 100,
                100, 80, 70, 60,    45,   60, 59, 70, 100,
                100, 80, 70, 60,    55,    60, 59, 70, 100]

    tracker.evalScan(testData)
    print(tracker.location)
    print(tracker.angle)
