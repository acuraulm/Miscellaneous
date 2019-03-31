import pickle

def register(conn, addr, nick):
	try:
		users = pickle.load(open("users","rb"))
	except:
		users = {}
		pickle.dump(users,open("users","wb"))
	try:
		nickname = users[addr[0]]['nick']
		conn.sendall('User already registered with nickname '+nickname)
	except:
		users[addr[0]] = {}
		users[addr[0]]['nick'] = nick
		users[addr[0]]['fileList'] = {}
		conn.sendall('You have been registered with nickname '+nick)
	
	pickle.dump(users,open("users","wb"))


def share(conn, addr, file):
	try:
		users = pickle.load(open("users","rb"))
	except:
		conn.sendall('You need to register first')
		return
	try:
		nickname = users[addr[0]]['nick']
	except:
		conn.sendall('You need to register first')
		return
	
	fileName = file.split(' ')[0]
	filePath = file.split(' ')[1]
	users[addr[0]]['fileList'][fileName] = filePath
	pickle.dump(users,open("users","wb"))
	conn.sendall('File '+fileName+' added')



def search(conn, addr, fileName, activePeers):
	try:
		users = pickle.load(open("users","rb"))
	except:
		conn.sendall('ERROR\nNo users registered till now')
		return

	usersHavingFile = {}
	userList = users.keys()
	for user in userList:
		found = False
		if fileName in users[user]['fileList'].keys():
			if user in activePeers:
				usersHavingFile[user] = {}
				usersHavingFile[user]['nick'] = users[user]['nick']
				usersHavingFile[user]['filePath'] = users[user]['fileList'][fileName]
	conn.sendall(str(usersHavingFile))


def checkDB(conn):
	try:
		users = pickle.load(open("users","rb"))
		conn.sendall(str(users))
	except:
		conn.sendall('File doesn\'t exist')