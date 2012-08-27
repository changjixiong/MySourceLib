import socket
import time

ser=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
ser.bind(('',9000))

State="Connected"

while True:    
    try:
        time.sleep(0.1)
        buf, addr =ser.recvfrom(512)
        if addr:
            print buf
            ser.sendto(buf, addr)
    except:
        pass

ser.close()
