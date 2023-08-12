extern crate glfw;
extern crate gl;

use glfw::{Action, Context, Key};
use std::{net::{TcpListener, TcpStream}, io::{BufReader, Read}};

const SERVER_ADDRESS: &str = "192.168.137.1:3000";

fn main() {
    let listener = TcpListener::bind(SERVER_ADDRESS).unwrap();
    
    println!("waiting for connection");
    for stream in listener.incoming() {
        handle_client(stream.unwrap());       
    }
    println!("done waiting");
    let mut glfw = glfw::init(glfw::FAIL_ON_ERRORS).unwrap();

    let (mut window, events) = glfw.create_window(800, 600, "Hello this is window", glfw::WindowMode::Windowed)
        .expect("Failed to create GLFW window.");


    window.set_key_polling(true);
    window.make_current();
    gl::load_with(|s| window.get_proc_address(s) as *const _);

    while !window.should_close() { 
        let y_vec: Vec<f64> = (0..1000).map(|val| val as f64).collect(); 
        let y_vals: [f64; 1000] = y_vec.try_into().unwrap();

        draw_graph(&y_vals, &[1.0; 1000]);
        window.swap_buffers();
        glfw.poll_events();

        for (_, event) in glfw::flush_messages(&events) {
            handle_window_event(&mut window, event);
        }
    }
}

fn handle_window_event(window: &mut glfw::Window, event: glfw::WindowEvent) {
    match event {
        glfw::WindowEvent::Key(Key::Escape, _, Action::Press, _) => {
            window.set_should_close(true)
        }
        _ => {}
    }
}

fn draw_graph(x_vals: &[f64; 1000], y_vals: &[f64; 1000]) {
    unsafe {
        gl::Clear(gl::COLOR_BUFFER_BIT);
    }
}

fn handle_client(mut stream: TcpStream) {
    let reader = BufReader::new(stream);

    let mut byte_index = 0;
    let mut data_index = 0;
    let mut single_reading = [0; 8];
    let mut readings = [0.0; 9];

    for byte in reader.bytes() {
        single_reading[byte_index] = byte.unwrap(); 
        
        byte_index += 1;

        if byte_index == 8 {
            readings[data_index] = f64::from_be_bytes(single_reading);

            data_index += 1;
            byte_index = 0;

            if data_index == 9 {
                println!("{:?}", readings);
                data_index = 0;
            }
        }
    }
}
