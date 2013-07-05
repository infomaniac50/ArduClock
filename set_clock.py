import serial
import time

def getRTCTime ():
	t = time.localtime()
	tfstr =	"M%-mD%-dY%Yh%-Im%-Ms%-S"
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

ser = serial.Serial('/dev/ttyACM0')

ser.drainOutput()

time.sleep(10)

setRTC(ser, getRTCTime())

ser.drainOutput()

ser.close()
