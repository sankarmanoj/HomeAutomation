import os,subprocess,thread
from serial import Serial
import serial
from mpd import MPDClient
import mpd
client = MPDClient()
client.connect("192.168.0.100",6600)
Xbee = Serial('/dev/ttyUSB0',9600)
def printer():
	out=""
	while True:
		a=Xbee.read(1)
		if a =="\n" or a=="!":
#			print "Recieved = ",out
			try:
				if "run" in out:
					if "stop" in out:
						client.stop()
					elif "volume" in out:
						if "change" in out:
							if "inc" in out:
								  client.setvol(int(client.status()["volume"])+1);
							elif "dec" in out:
								client.setvol(int(client.status()["volume"])-1);
						else:
	        	                                client.setvol(int(out.split("=")[-1]))
					elif "play" in out:
						client.play()
					elif "prev" in out:
						client.previous()
					elif "next" in out:
						client.next()
					elif "pause" in out:
						client.pause()
			except Exception as inst:
				print inst
				print type(inst)
				if type(inst)==serial.SerialException:
					print "Error"
					client.setvol(100)
					sleep(3)
					client.setvol(0)
					os.system("sudo reboot")
				if type(inst)==mpd.ConnectionError:
					client.close
					client.connect("192.168.0.100",6600)							
			out = ""
		else:
			out+=a
printer()
