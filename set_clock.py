import serial
import time
import sys

def getRTCTime ():
	t = time.localtime()
	tfstr =	"M%mD%dY%Yh%Im%Ms%S"
	ts = time.strftime(tfstr, t)
	pm = time.strftime("%p", t)


	if pm == 'AM':
		ts += 't0'
	elif pm == 'PM':
		ts += 't1'
	else:
		ts += 't0'

	ts += '+'

	return 's' + ts

def setRTC (ser,t):
	ser.write(t)
	time.sleep(2)

ser = serial.Serial('COM6')

ser.flushOutput()

print('Wait 10 seconds since DTR resets MCU\n')

time.sleep(10)

t = getRTCTime()

sys.stdout.write('Sending time string: ')
print(t)

setRTC(ser, bytearray(t ,'ascii'))

ser.flushOutput()

print('\nClockduino Output:\n')

sys.stdout.write(str(ser.readline(), 'ascii'))
sys.stdout.write(str(ser.readline(), 'ascii'))
sys.stdout.write(str(ser.readline(), 'ascii'))

ser.close()

