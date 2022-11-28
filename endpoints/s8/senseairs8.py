import serial
import time

ser = serial.Serial("/dev/ttyS0", baudrate = 9600, timeout = 0.5)
ser.flushInput()
time.sleep(1)

ser.write(b"\xFE\x04\x00\x03\x00\x01\xD5\xC5")
time.sleep(.5)
resp = ser.read(7)
print(((resp[3] << 8) + resp[4]))
