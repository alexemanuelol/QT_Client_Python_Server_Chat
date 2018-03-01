import socket
import select
import time

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

print("Listening for port {}".format(port))

while True:
	tup = select.select(listOfSockets, [], [])
	sock = tup[0][0]

	if sock == sockL:
		sockClient, addr = sockL.accept()
		dictOfAddresses[sockClient] = addr
		listOfSockets.append(sockClient)
	else:
		localTime = time.strftime("%H:%M")
		data = sock.recv(1000)
		if not data or data.decode("utf-8").startswith("disconnect%s:%s" % dictOfAddresses[sock]):
			length = len("disconnect%s:%s" % dictOfAddresses[sock])
			data = data.decode("utf-8")
			nickname = data[length:]
			broadcast_data("[{}] ".format(localTime) + nickname + " has gone offline!")
			sock.close()
			listOfSockets.remove(sock)
			del dictOfAddresses[sock]
		else:
			broadcast_data("[{}] ".format(localTime) + data.decode("utf-8"))

sockL.close()