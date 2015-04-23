__author__ = 'Angelo'
import serial
import sys, getopt
import time

CONST_LEFT_COMMAND = "Left"
CONST_RIGHT_COMMAND = "Right"
direction = ''
steps = ''


try:
    opts, args = getopt.getopt(sys.argv[1:],"d:s:")
except getopt.GetoptError as e:
    print (str(e))
    print("Usage: %s -d direction -s steps" % sys.argv[0])
    sys.exit(2)
for opt, arg in opts:
    if opt == '-d':
        direction = arg
    elif opt == '-s':
        steps = arg

print ("Direction: %s and Steps: %s" % (direction,steps) )

if direction.lower() == 'links':
    direction = CONST_LEFT_COMMAND
else:
    direction = CONST_RIGHT_COMMAND

ser = serial.Serial('/dev/ttyACM0', 9600)


command = ('%s\n' % (direction))

ser.write(command)

time.sleep(1)

command = ('%s' % (steps))

ser.write(command)
