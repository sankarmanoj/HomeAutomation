import boto.sqs 
from boto.sqs.message import Message
from time import sleep
conn = boto.sqs.connect_to_region("us-west-2",aws_access_key_id='AKIAIJN6KR6TYDVTTPRQ',aws_secret_access_key='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')
droplet = conn.get_queue('droplet')

while True:
	sleep(0.3)
	if(droplet.count()>0):
			try:
				m = droplet.read()
				print m.get_body()
				droplet.delete_message(m)
			except:
				pass