import socket

HOST = "172.20.10.7"  # Standard loopback interface address (localhost)
PORT = 3000  # Port to listen on (non-privileged ports are > 1023)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    print("binding now")
    s.bind((HOST, PORT))
    print("listening")
    s.listen()
    conn, addr = s.accept()
    with conn:
        print(f"Connected by {addr}")
        while True:
            data: bytes = conn.recv(9)

            if not data:
                break
            
            print(list(data))
