import socket
import sys
import threading
import pickle

try:
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
except socket.error, msg:
	print 'Failed to create socket. Error code: ' + str(msg[0]) + ' ,Error message: '+ msg[1]
	sys.exit()

sListen=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sListen.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
portL = 8888

try:
	sListen.bind(('',portL))
except socket.error, msg:
	print 'Bind failed. Error Code: ' + str(msg[0]) + ' Message: ' + msg[1]
	sys.exit()
sListen.listen(10)
print "Socket now listening"


def client(host, port, s, portL):
	try:
		remote_ip = socket.gethostbyname(host)
	except socket.gaierror:
		print 'Hostname couldn\'t be resolved. Exiting'
		sys.exit()
	
	s.connect((remote_ip, port))
	print 'Socket connected to ' + host + ' on ip ' + remote_ip
	reply = s.recv(4096)
	print reply
	
	handleInput()
	
	s.close()
	
	
#Handle user input
def handleInput():
	quit = False
	while not quit:
		input = raw_input(">> ")
		if not input:
			continue
		elif input[0] is 'U':
			fileName = raw_input('Enter file name: ')
			filePath = raw_input('Enter path: ')
			message = 'SHARE_FILES\n'+fileName+' '+filePath
	
		elif input[0] is 'R':
			nickname = raw_input('Enter a nickname: ')
			message = 'REGISTER\n'+nickname
		
		elif input[0] is 'S':
			fileName = raw_input('Enter file name to be searched: ')
			message = 'SEARCH\n'+fileName
			try:
				s.sendall(message)
			except socket.error:
				print 'Send failed'
				sys.exit()
			reply = s.recv(4096)
			if reply.split('\n')[0] == 'ERROR':
				print reply.split('\n')[1]
				sys.exit()

			usersHavingFile = eval(reply)
			if not usersHavingFile:
				print 'File not found'
				continue
			
			message = 'The following users have the file:\n'
			for user in usersHavingFile.keys():
				message = message + usersHavingFile[user]['nick'] + ' (' + user + ') (' + usersHavingFile[user]['filePath'] + ')\n'
			print message
			
			tryDownload(fileName, usersHavingFile)
			continue
			
		elif input is 'E':
			quit = True
			continue
			
		else:
			print 'Unknown command'
			continue
	
		try:
			s.sendall(message)
		except socket.error:
			print 'Send failed'
			sys.exit()
		
		reply = s.recv(4096)
		print reply

def tryDownload(fileName, usersHavingFile):
	response = raw_input('Write \"Q\" followed by the client IP for downloading file from that client\n')
	response = response.strip()
			
	if response[0] == 'Q':
		s1 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		peerIP = response.split(' ')[1]
		s1.connect((peerIP, portL))
		queryMessage = 'DOWNLOAD\n' + fileName + '\n' + usersHavingFile[peerIP]['filePath']
		try:
			s1.sendall(queryMessage)
		except socket.error:
			print 'Send failed'
			sys.exit()
		fw = open('Received'+fileName,'wb+')
		flag = 0
		chunk = s1.recv(100)
		
		while chunk.strip() != 'SHUT_WR':
			s1.send('received')
			if chunk.split('\n')[0] == 'ERROR':
				print chunk.split('\n')[0]+' '+chunk.split('\n')[1]
				flag = 1
				break
			fw.write(chunk)
			chunk = s1.recv(100)
		if flag != 1:
			print "\nFile saved in current folder"
		fw.close()
		s1.close()
		
def listenForSharing(sListen):
	
	while 1:
		conn, addr = sListen.accept()
		data = conn.recv(1024)
		if data.split('\n')[0]=='DOWNLOAD':
			fileName = data.split('\n')[1]
			filePath = data.split('\n')[2]
			print filePath+fileName
			try:
				fr = open(filePath+fileName,'rb')
			except:
				conn.sendall('ERROR\nNo such file available')
				continue
			chunk = fr.read()
			conn.send(chunk)
			ack = conn.recv(100)
			conn.sendall('SHUT_WR')
			fr.close()
	sListen.close()


try:
	host = sys.argv[1]
	port = int(sys.argv[2])
	print host
	print port
	
	if __name__ == '__main__':
		t = threading.Thread(target = client, args=(host,port,s,portL))
		t.start()
		t2 = threading.Thread(target = listenForSharing, args = (sListen,))
		t2.daemon = True
		t2.start()
except:
	sListen.close()