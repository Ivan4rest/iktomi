import serial

ser = serial.Serial('COM5', 9600)

while True:
    data = ser.readline().decode('utf-8').strip()
    print(data)
    with open('data.txt', 'a') as f:
        f.write(data + '\n')
