

class RingBuffer:
    def __init__(self, max_size):
        self._max_size = max_size
        self._head = 0
        self._tail = 0
        self._buffer = [0] * self._max_size
    
    def write(self, data):
        next_pos = self._head + 1
        if (next_pos == self._max_size):
            next_pos = 0
        if (next_pos != self._tail):
            self._buffer[self._head] = data
            print(f"Write: Head Index= {self._head}, Value= {data}, Next Head Pos= {next_pos}")
            self._head = next_pos
        else:
            print("Buffer is full on next write")

    def read(self):
        temp = 0
        next_pos = self._tail + 1
        if (self._tail != self._head):
            if (next_pos == self._max_size):
                next_pos = 0
            temp = self._buffer[self._tail]
            print(f"Read: Tail Index= {self._tail}, Value= {temp}, Next Head Pos= {next_pos}")
            self._tail = next_pos
        else:
            print("Buffer is empty")
        return temp

    def data_available(self):
        available = False
        if (self._head != self._tail):
            available = True
        return available




""" 
#################### TEST CODE #################
def main():
    ring = RingBuffer(10)

    for i in range(1, 7):
        ring.write(i)

    print("------------------------------------------------------------------------")
    
    if (ring.data_available()):
        for j in range(1, 3):
            x = ring.read()
            

    print("------------------------------------------------------------------------")

    if (ring.data_available()):
        for j in range(1, 5):
            y = ring.read()

    print("------------------------------------------------------------------------")

    for i in range(1, 7):
        ring.write(i)



    if (ring.data_available()):
        for j in range(1, 12):
            y = ring.read()
    
    for i in range(15):
        ring.write(i)

main() 
"""