import serial
from time import sleep

from ring_buffer import RingBuffer

def send_data(uart, msg):
    success = uart.write(msg.encode('utf-8'))
    uart.flush()
    sleep(0.001)
    return success

def recieve_data(uart, msg_size):
    rx = uart.read(msg_size)
    sleep(0.001)
    return rx


def main():
    uart = serial.Serial(port='COM5', baudrate=9600, bytesize=8, stopbits=1, timeout=1)  # open serial port
    tx_buffer = RingBuffer(32)
    rx_buffer = RingBuffer(32)
    if (uart.is_open):
        print("open")
    
    # Test 1: Data frame with uncorrupt data
    #msg = "s11&r"
    #msg = "s12\x16\x11"
    #msg_size = 1
    #msg = "s4abcd\xA8\x36"
    #msg_size = 4
    #msg = "s9123456789\x31\xC3"
    #msg_size = 9
    #error_data = 0

    # Test 2: Data frame with corrupt data
    #msg = "s12&r"
    #msg = "s13\x16\x11"
    #msg_size = 1
    #msg = "s4abfd\xA8\x36"
    #msg_size = 4
    msg = "s912y456789\x31\xC3"
    #msg = "s912y456i89\x31\xC3"
    msg_size = 9
    error_data = 1

    # Test 1: Data frame with corrupt CRC
    #msg = "s11&t"
    #msg_size = 1
    #msg = "s12\x13\x11"
    #msg_size = 1
    #msg = "s4abcd\xA7\x32"
    #msg_size = 4
    #msg = "s9123456789\x32\xC4"
    #msg_size = 9
    #error_data = 1

    frame_header = 1
    msg_len = 1
    crc_len = 2
    frame_len = frame_header + msg_size + msg_len + crc_len

    # rx = []

    sleep(1) # ESSENTIAL: A delay is required between to allow the COM port to be created to the Arduino.
    """  
    send_data(uart, msg)    
    for i in range(msg_len):
        rx = uart.read(1)
        print(rx) 
    """
    ###############################
    print("-------- Transmitter ----------")
    for i in range(frame_len):
        print(i)
        tx_buffer.write(msg[i])
        if (tx_buffer.data_available()):
            send_data(uart, tx_buffer.read())

    print("-------- Reciever ----------")
    for i in range(frame_len-2 + error_data):
        rx_buffer.write(uart.read(1))
        if (rx_buffer.data_available()):
            print(rx_buffer.read())
    
    # print(msg)
    sleep(0.01)
    uart.close()

    #print(rx)
    
    # Works - OLD CODE
    """msg = recieve_data(uart, 1)
    print(msg)

    if (send_data(uart, msg)):
        print("Msg Sent!")

    msg = recieve_data(uart, 1)
    print(msg) 
    uart.close() """

main()