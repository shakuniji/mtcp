#!/usr/bin/env python

import socket
import datetime
from multiprocessing import Pool

def sendserver(x):
	print x
	TCP_IP = '10.0.0.4'
	TCP_PORT = 8000
	BUFFER_SIZE = 1024
	MESSAGE = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"

	endTime = datetime.datetime.now() + datetime.timedelta(minutes=1)
	count=0
	while(datetime.datetime.now() <= endTime):
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		#s.bind(("10.0.0.5",0))
		s.connect((TCP_IP, TCP_PORT))
		s.send(MESSAGE)
		data = s.recv(BUFFER_SIZE)
		count+=1

		#s.close()
	print "Requests per second ", count/60

if __name__=='__main__':
	p = Pool(50)
	p.map(sendserver,[x for x in range(15)])
