import socket
import sys
from thread import *
import functions

host = '10.132.0.2'
port = 55555

s=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
print 'Socket created'
try:
	s.bind((host,port))
except socket.error, msg:
	print 'Bind failed. Error Code: ' + str(msg[0]) + ' Message: ' + msg[1]
	sys.exit()

print 'Socket bind complete'

s.listen(10)
print 'Socket now listening'
activePeers = []
users = {}
def clientthread(conn,addr):
	conn.send('Commands:\n 1. R - register with an username to the server\n 2. U - upload a file to the server\n \
		3. S - search for the given file. Check if any peer has it\n 4. E - exit peer client')
	activePeers.append(addr[0])
	while 1: 
	    data = conn.recv(1024)
	    if not data:
	    	break

	    if data.split('\n')[0] == 'REGISTER':
	    	functions.register(conn, addr, data.split('\n')[1])
	    elif data.split('\n')[0] == 'SHARE_FILES':
	    	functions.share(conn,addr,data.split('\n')[1])
	    elif data.split('\n')[0] == 'SEARCH':
	    	functions.search(conn,addr,data.split('\n')[1],activePeers)

	activePeers.remove(addr[0])
	conn.close()


while 1:
	conn, addr = s.accept()
	print 'Connected with ' + addr[0] + ':' + str(addr[1])

	start_new_thread(clientthread, (conn,addr))


s.close()