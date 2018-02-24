import socket
import select

def broadcast_data(message):
	for socket in listOfSockets:
		if socket != sockL:
			socket.send(bytearray(message, "utf-8"))

port = 60000
sockL = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sockL.bind(("", port))
sockL.listen(10)

listOfSockets = [sockL]
dictOfAddresses = {}

print("Lyssnar på port {}".format(port))

while True:
	tup = select.select(listOfSockets, [], [])
	sock = tup[0][0]

	if sock == sockL:
		sockClient, addr = sockL.accept()
		dictOfAddresses[sockClient] = addr
		listOfSockets.append(sockClient)
		broadcast_data("[%s:%s] (Uppkopplad)" % dictOfAddresses[sockClient])
	else:
		data = sock.recv(1000)
		if not data or data.decode("utf-8") == "disconnect%s:%s" % dictOfAddresses[sock]:
			broadcast_data("[%s:%s] (Nedkopplad)" % dictOfAddresses[sock])
			sock.close()
			listOfSockets.remove(sock)
			del dictOfAddresses[sock]
		else:
			broadcast_data("[%s:%s] " % dictOfAddresses[sock] + data.decode("utf-8"))

sockL.close()