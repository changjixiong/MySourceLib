import socket

address=('127.0.0.1',9000)

client=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
client.connect(address)

while True:    
    try:        
        client.sendall("client data")
        buf=client.recv(512)
        print buf
    except:
        client.connect(address)
