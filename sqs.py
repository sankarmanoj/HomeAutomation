import boto.sqs 
from boto.sqs.message import Message
conn = boto.sqs.connect_to_region("us-west-2",aws_access_key_id='AKIAIJN6KR6TYDVTTPRQ',aws_secret_access_key='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')
droplet = conn.get_queue('droplet')
input=""
while input!='q':

	input = raw_input("Enter input")
	if input=='p':
		droplet.purge()
	m = Message()
	m.set_body(input)
	droplet.write(m)