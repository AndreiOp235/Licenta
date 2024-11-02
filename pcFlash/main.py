import serial

ser = serial.Serial("COM3", 115200)

# Read line
while True:
    bs = ser.readline()
    print(bs)