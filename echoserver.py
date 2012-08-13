import socket
import time

ser=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
ser.bind(('',9000))
ser.listen(5)
connection,address=ser.accept()

State="Connected"

while True:    
    try:
        time.sleep(0.1)
        buf=connection.recv(512);
	print buf 
        connection.send(buf)
    except:
        connection,address=ser.accept()
