import socket 

host = '10.0.0.2' 
port = 8000 
backlog = 512 
size = 8192 
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM) 
s.bind((host,port)) 
s.listen(backlog) 
client, address = s.accept() 
while 1: 
    data = client.recv(size) 
