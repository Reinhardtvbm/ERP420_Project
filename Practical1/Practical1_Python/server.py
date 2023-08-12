import socket
import numpy as np

HOST = "192.168.137.1"  # Standard loopback interface address (localhost)
PORT = 3000  # Port to listen on (non-privileged ports are > 1023)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    print("binding now")
    s.bind((HOST, PORT))

    print("listening")
    s.listen()

    conn, addr = s.accept()

    with conn:
        print(f"Connected by {addr}")

        byte_index = 0
        data_index = 0

        # one reading of the 9 coming from the sensor (made up of 4 bytes)
        single_reading = np.zeros(4, np.uint8)

        # all nine readings
        readings =  np.zeros(9, np.int32)

        while True:
            # wait to receive one byte
            data: bytes = conn.recv(1)

            if not data:
                break
            
            # only reading one byte at a time on line 29, but it returns a list that contains that one byte
            single_reading[byte_index] = int(list(data)[0]); 

            byte_index += 1

            if byte_index == 4:
                # bytes are being sent in little endian format
                reading = int.from_bytes(single_reading, byteorder='little', signed=True)
                readings[data_index] = reading

                data_index += 1
                byte_index = 0

                if data_index == 9:
                    # now we have all 9 readings here
                    print(readings)
                    data_index = 0
