import time
import sys
import glob
import serial


def serial_ports():
    """Lists serial ports

    :raises EnvironmentError:
        On unsupported or unknown platforms
    :returns:
        A list of available serial ports
    """
    if sys.platform.startswith('win'):
        ports = ['COM' + str(i + 1) for i in range(256)]

    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this is to exclude your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')

    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')

    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result
toAr = serial.Serial(serial_ports()[-1])
print toAr.name
recv = ""
init = False
oldval =1
val = 10
start = time.time()
while True:
    time.sleep(0.01)
    if(toAr.inWaiting()>0):
        recv +=toAr.read(1)
        print recv
    if (recv=="started"):
        print "Initialsed with Arduino"
        recv = ""  
        init = True
    if init:      
        try:
            a= file(r"C:\wamp\www\val.txt","r")
            val = int(a.read())            
            a.close()
        except:
            pass
        if val !=oldval:
            toSend = "~"+str(val)+"!\n"
            print "Sent ", toSend
            toAr.write(toSend)
            oldval = val
       
