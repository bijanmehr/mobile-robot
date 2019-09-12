#!/usr/bin/env python

import socket
import time

def sender(msg):
    TCP_IP = '192.168.1.100'
    TCP_PORT = 80
    BUFFER_SIZE = 1024
    # msg = bytes(msg, 'utf-8')
    MESSAGE = msg

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((TCP_IP, TCP_PORT))
    s.send(MESSAGE)
    data = s.recv(BUFFER_SIZE)
    s.close()

    print("received data:", data)


if __name__ == "__main__":
    sender("MR-1023L-1023*")
    # time.sleep(1)
    # sender("MR-1024L1024*")
    # time.sleep(1)
    # sender("MR0L0*")
    # time.sleep(1)
    # sender("MR1023L1023*")
    # time.sleep(1)
    # sender("MR0L0*")
    
