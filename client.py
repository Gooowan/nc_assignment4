#!/usr/bin/env python3

import socket
import sys

def send_text(sock, text):
    text_encoded = text.encode('utf-8')
    text_size = len(text_encoded)
    sock.sendall(text_size.to_bytes(4, byteorder='big'))
    sock.sendall(text_encoded)

def receive_text(sock):
    size_data = sock.recv(4)
    if not size_data:
        print("Connection closed by server.")
        sys.exit()
    text_size = int.from_bytes(size_data, byteorder='big')
    text = sock.recv(text_size).decode('utf-8')
    return text

def main(server_ip, server_port):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((server_ip, server_port))
        
        first_message = input("Enter your message: ")
        send_text(sock, first_message)

        response1 = receive_text(sock)
        print("Received:", response1)
        
        response2 = receive_text(sock)
        print("Received:", response2)
        
        second_message = input("Enter your message: ")
        send_text(sock, second_message)
        
        final_response = receive_text(sock)
        print("Received:", final_response)

        print("Server has closed the connection.")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python client.py <Server IP> <Server Port>")
        sys.exit()
    
    server_ip = sys.argv[1]
    server_port = int(sys.argv[2])
    main(server_ip, server_port)
