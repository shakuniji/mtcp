#!/usr/bin/env python

import socket
import struct
import datetime
from multiprocessing import Pool


def request(x):
	count =0
	end = datetime.datetime.now() + datetime.timedelta(seconds = 300)
	while (datetime.datetime.now() < end ):
		try:
			sendserver(x)
			count +=1
		except:
			pass
	return count


def sendserver(x):
	TCP_IP = '10.0.20.2'
	TCP_PORT = 9000
	BUFFER_SIZE = 1024
	MESSAGE = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
	
	dt = datetime.datetime(2016,1,29,15,29,30)                                                      
	endTime = datetime.datetime.now() + datetime.timedelta(minutes=1)
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	#s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	l_onoff = 1
	l_linger = 0
	#s.setsockopt(socket.SOL_SOCKET, socket.SO_LINGER,struct.pack('ii', l_onoff, l_linger))
	s.bind(("10.0.20.3",0))
	s.connect((TCP_IP, TCP_PORT))
	s.send(MESSAGE)
	data = s.recv(BUFFER_SIZE)
	#print data
		
	s.close()

if __name__=='__main__':
	p = Pool(50)
	x = p.map(request,[x for x in range(40)])
	print sum(x)
