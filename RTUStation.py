if __name__=="__main__":

    import socket
    import time
    import random
    import logging     
    import datetime    
    logger = logging.getLogger()
    #hdlr = logging.FileHandler('./log.txt')
    logger.handler.RotatingFileHandler('./log.txt','a',1000)
    formatter = logging.Formatter('%(asctime)s %(levelname)s %(message)s')
    hdlr.setFormatter(formatter)
    logger.addHandler(hdlr)
    logger.setLevel(logging.NOTSET)

    
    ser=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
    ser.bind(('127.0.0.1',404))
    ser.listen(5)

    connection,address=ser.accept()
    State="Connected"
    

    buf=""
    listSOEHead=['\x02ON ',
             '\x02OFF ',
             '\x02ON ',
             '\x02OFF ',
             '\x02OFF ',
             '\x02ON ']

    listSOEtail=[' PT 0913 Status Input Pt #913\x00\x0D\x0A\x03',
             ' PT 0058 Status Input Pt #58 (SOE)\x00\x0D\x0A\x03',
             ' PT 0113 A1 LOSS OF BOILER WTR B OPERATED\x00\x0D\x0A\x03',
             ' PT 0113 A1 LOSS OF BOILER WTR B OPERATED\x00\x0D\x0A\x03',
             ' PT 2098 011NUFLScommFAIL34,36,37,p8\x00\x0D\x0A\x03',
             ' PT 2098 011NUFLScommFAIL34,36,37,p8\x00\x0D\x0A\x03']
    SoeTotalCount=0

    while True:
        try:
            time.sleep(0.1)
            if State=="Connected":
                connection.send('\r\nUser ID: ')
                State="WaitUserName"

                continue
            
            elif State=="WaitUserName":
      
                buf=connection.recv(512);
                print buf
               
                if buf.find('ds')>=0:
                    connection.send('ds\r\r\nPassword: ')
                    State="WaitPassword"
                else:
                    print "WaitUserName error"
                        
                continue
            
            elif State=="WaitPassword":
                
                buf=connection.recv(512);
                print buf

                if buf.find('ds')>=0:
                    connection.send('**\x0D')
                    State="Work"

                    
                continue
            
            elif State=="Work":

                buf=connection.recv(512)
                print buf

                if random.randint(1,10)==1:
                    connection.send('\x02\x1B5\x03')
                    connection.send('\x02\x1B5\x03')
                    connection.send('\x02\x1B5\x03')
                    connection.send('\x02\x1B5\x03')
                    connection.send('\x02\x1B5\x03')
                elif random.randint(1,10)==2:
                    connection.send('\x02\x1BCB\x1BN\x06\x03')
                elif random.randint(1,10)==3:
                    connection.send(listSOEHead[random.randint(0,len(listSOEHead)-1)]+(str(datetime.datetime.now()))[0:22]+listSOEtail[random.randint(0,len(listSOEtail)-1)])
                else:
                    connection.send(listSOEHead[random.randint(0,len(listSOEHead)-1)]+(str(datetime.datetime.now()))[0:18]+listSOEtail[random.randint(0,len(listSOEtail)-1)])

                    SoeTotalCount=SoeTotalCount+1
                    logger.info(str(SoeTotalCount))
        except:
            connection,address=ser.accept()
            State="Connected"


            

            
