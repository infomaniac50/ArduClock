import serial
import time

def getRTCTime ():
	t = time.localtime()
	tfstr =	"M%-mD%-dY%Yh%-Hm%-Ms%-S"
	ts = time.strftime(tfstr, t)

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
