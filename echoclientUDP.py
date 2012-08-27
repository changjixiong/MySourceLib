import socket

address=('127.0.0.1',9000)

client=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)


while True:    
    try:        
        client.sendto("client data", address)
        buf, addr=client.recvfrom(512)
        if buf:
            print buf
    except:
        pass


