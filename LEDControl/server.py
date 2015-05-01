from socket import * 
import thread
from time import sleep 
from serial import Serial 
toAr = Serial('/dev/ttyACM0',9600)
value = "100"
server = socket(SOCK_DGRAM,AF_INET)
server.bind(('',12321)) 
send_size=1024 
i = None 
import boto.sqs 
from boto.sqs.message import Message 
conn = boto.sqs.connect_to_region("us-west-2",aws_access_key_id="",aws_secret_access_key="") 
qs = conn.get_all_queues()
for q in qs:
	print q.name
droplet = conn.get_queue('droplet')
import thread
ads=[]
value = 0
def sqs_listener():
	droplet = conn.get_queue('droplet')
	print droplet
	data = ""
	while True:
		sleep(0.3)
		if(droplet.count()>0):
			try:
				m = droplet.read()
				data =  m.get_body()
				print "Got ",data

				droplet.delete_message(m)
			except Exception as inst:
				print type(inst)     # the exception instance
   				print inst.args
				print inst 
			value = (data.split("="))[-1]
			toSend = value + "~"
			print toSend
			toAr.write(toSend)
thread.start_new_thread(sqs_listener,())
def ArReader():
	inString = ""
	while True:	
		if(toAr.inWaiting()>0):
			inchar = toAr.read(1)
			if(inchar=="~"):
				try:
					value = int(inString)
					inString = ""
					for ad in ads:
						server.sendto("value="+str(value),ad)	
				except Exception as e:
					print e
			else:
				inString+=inchar
			
thread.start_new_thread(ArReader,())
def delay_off():
	print "Thread started"
	sleep(10)
	value="0"
	toAr.write(value+"~")
print "Listening"

while True:
	data,addr = server.recvfrom(1000)
	if addr not in ads:
		ads.append(addr)
	if "value" in data:
		value = (data.split("="))[-1]
		if int(value)>100:
			value = str(100)
		if int(value)<0:
			value = str(0)
		toSend = value + "~"
		toAr.write(toSend)
		server.sendto(data,addr)
	if "request" in data:
		server.sendto(value,addr)
		
