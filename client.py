#!/usr/bin/env python

import socket
import struct
import datetime
from multiprocessing import Pool

def sendserver(x):
	print x
	TCP_IP = '10.0.0.4'
	TCP_PORT = 8000
	BUFFER_SIZE = 1024
	MESSAGE = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
	
	dt = datetime.datetime(2016,1,29,15,22,30)                                                      
	while (datetime.datetime.now() < dt):continue
	
	endTime = datetime.datetime.now() + datetime.timedelta(minutes=1)
	count=0
	while(datetime.datetime.now() <= endTime):
		try:
			s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
			#s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
			l_onoff = 1
			l_linger = 0
			s.setsockopt(socket.SOL_SOCKET, socket.SO_LINGER,struct.pack('ii', l_onoff, l_linger))
			#s.bind(("10.0.0.5",0))
			s.connect((TCP_IP, TCP_PORT))
			s.send(MESSAGE)
			data = s.recv(BUFFER_SIZE)
			count+=1

			#s.close()
		except:
			print 'e'
	print "Requests per second ", count/60
	return count/60

if __name__=='__main__':
	p = Pool(50)
	nump = 40
	x = p.map(sendserver,[x for x in range(nump)])
	print sum(x)/nump
