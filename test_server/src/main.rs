use std::{net::{TcpListener, TcpStream}, io::{BufReader, Read}};

const SERVER_ADDRESS: &str = "192.168.137.1:3000";

fn main() {
    let listener = TcpListener::bind(SERVER_ADDRESS).unwrap();
    
    println!("waiting for connection");
    for stream in listener.incoming() {
        handle_client(stream.unwrap());       
    }
    println!("done waiting");
}

fn handle_client(mut stream: TcpStream) {
    let reader = BufReader::new(stream);

    let mut byte_index = 0;
    let mut data_index = 0;
    let mut single_reading = [0; 4];
    let mut readings = [0.0; 9];

    for byte in reader.bytes() {
        single_reading[byte_index] = byte.unwrap(); 
        
        byte_index += 1;

        if byte_index == 4 {
            readings[data_index] = f32::from_le_bytes(single_reading);

            data_index += 1;
            byte_index = 0;

            if data_index == 9 {
                println!("{:?}", readings);
                data_index = 0;
            }
        }
    }
}