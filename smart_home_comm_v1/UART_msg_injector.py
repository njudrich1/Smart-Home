import serial
from time import sleep

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
    if (uart.is_open):
        print("open")
    
    # Test 1: Data frame with uncorrupt data
    #msg = "s11&r"
    #msg = "s12\x16\x11"
    #msg = "s4abcd\xA8\x36"
    #msg = "s9123456789\x31\xC3"

    # Test 2: Data frame with corrupt data
    #msg = "s12&r"
    #msg = "s13\x16\x11"
    #msg = "s4abfd\xA8\x36"
    #msg = "s912y456789\x31\xC3"
    #msg = "s912y456i89\x31\xC3"

    # Test 1: Data frame with corrupt CRC
    #msg = "s11&t"
    #msg = "s12\x13\x11"
    #msg = "s4abcd\xA7\x32"
    msg = "s9123456789\x32\xC4"

    crc_len = 2
    DBG_error_indic_len = 1
    msg_len = 9 + crc_len + DBG_error_indic_len

    rx = []

    sleep(1) # ESSENTIAL: A delay is required between to allow the COM port to be created to the Arduino.
    send_data(uart, msg)    
    for i in range(msg_len):
        rx = uart.read(1)
        print(rx)
        #print("Msg Sent!")
    
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