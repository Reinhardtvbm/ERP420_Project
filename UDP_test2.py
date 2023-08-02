
import socket

HOST = "192.168.0.178"
PORT = 3000

UDPServerSocket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)
UDPServerSocket.bind((HOST, PORT))

while True:
    readings = UDPServerSocket.recv(20)

    if len(readings) >= 18:
        print([[readings[i], readings[i + 1]] for i in range(0, len(readings), 2)])
