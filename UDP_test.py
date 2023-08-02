import socket

HOST = "192.168.0.178"
PORT = 3000

UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
UDPServerSocket.bind((HOST, PORT))

while True:
    sync_packet = [0, 0]

#    while sync_packet != [255, 255]:
    sync_packet[0] = UDPServerSocket.recv(1)[0]
    sync_packet[1] = UDPServerSocket.recv(1)[0]

    readings = []

    readings.append(sync_packet[0])
    readings.append(sync_packet[1])

    for _ in range(18):
        readings.append(UDPServerSocket.recv(1)[0])

    print([[readings[i], readings[i + 1]] for i in range(0, len(readings), 2)])
