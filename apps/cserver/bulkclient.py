#!/usr/bin/env python

import socket
import struct
import datetime,time
from multiprocessing import Pool

filelength = 1024 * 1024 * 1024 # 1 GB

MESSAGE_SIZE = 1448
MESSAGE = ''

for i in range(MESSAGE_SIZE):
	MESSAGE += 'a'

def sendserver(x):
	TCP_IP = '10.0.0.2'
	TCP_PORT = 8000
	BUFFER_SIZE = 1024
	
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	#s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
	l_onoff = 1
	l_linger = 0
	#s.setsockopt(socket.SOL_SOCKET, socket.SO_LINGER,struct.pack('ii', l_onoff, l_linger))
	s.bind(("10.0.0.3",0))
	s.connect((TCP_IP, TCP_PORT))
	sent = 0
	st = time.time()
	while sent < 5 * filelength:
		s.send(MESSAGE)
		sent += len(MESSAGE)
		
	#data = s.recv(BUFFER_SIZE)
	end = time.time()
	print 'done throughput = ', (5*filelength)/((end - st)*1024*1024*1024) 
		
	s.close()
	return sent

if __name__=='__main__':
	poolsize = 1
	p = Pool(poolsize)
	y = p.map(sendserver,[x for x in range(poolsize)])
	print sum(y)
