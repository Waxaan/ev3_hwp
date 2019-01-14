from ev3dev2.sound import Sound

A1 = 440
SCALE = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "B", "H"]
NOTE_LENGTH_MS = 500


class Note:
    def __init__(self, name, octave=1, length=1, delay=0.01):
        self.scalePos = SCALE.index(name) - 9
        self.octave = octave
        self.length = length * NOTE_LENGTH_MS
        self.delay = delay * NOTE_LENGTH_MS


def convert(notes):
    result = []
    for note in notes:
        frequency = A1 * pow(pow(2, 1 / 12), note.scalePos) * \
            pow(2, note.octave)
        result.append((frequency, note.length, note.delay))
    return result


def transpose(notes, shift):
    result = []
    for note in notes:
        result.append((note[0] * pow(pow(2, 1 / 12), shift), note[1], note[2]))
    return result


notes_start = [
    Note('G'), Note('C', 2), Note('G'), Note('C'),
    Note('G'), Note('C', 2), Note('G', length=1.5, delay=0.5),
    Note('G'), Note('C', 2), Note('G'), Note('C', 2),
    Note('E', 2, 1.5), Note('D', 2, 0.5), Note('C', 2, 0.5),
    Note('H', length=0.5), Note('A', length=0.5), Note('G#', length=0.5),

    Note('G'), Note('C', 2), Note('G'), Note(
        'E', length=0.5), Note('F', length=0.5),
    Note('G'), Note('C', 2), Note('G', length=1, delay=1),
    Note('C', 2, 0.5, 1), Note('A', length=0.5), Note('G'), Note('F')
]

notes_end = [Note('E'), Note('D'), Note('C', length=2)]

ev3Notes = convert(notes_start + notes_end)
ev3Notes.extend(transpose(ev3Notes, 3))

for i in range(-3, -1):
    note = ev3Notes[i]
    ev3Notes[i] = (note[0], note[1] * 2, note[2] * 2)


def play():
    sound = Sound()
    sound.tone(ev3Notes)


if __name__ == '__main__':
    play()
