import datetime
import time
import os

class MyLog:
    def __init__(self, prefix, logSize):
        self.prefix = prefix
        self.logSize = logSize
        self.recordFile = None
        self.recordName = ''

    def MakeFileName(self):
        return self.prefix\
               +datetime.datetime.now().strftime("%Y%m%d%H%M%S%f")[0:17]\
               +'.txt'
        
    def witreLog(self, content, closeAfterWrite=False):
        if self.recordName=='':
            self.recordName=self.MakeFileName()

        if self.recordFile == None or self.recordFile.closed:
            self.recordFile = file(self.recordName,'a')
        
        if os.path.getsize(self.recordName)>= self.logSize:
            self.recordFile.close()
            self.recordName=self.MakeFileName()
            self.recordFile = file(self.recordName,'a')

        self.recordFile.write(str(datetime.datetime.now())[0:23])
        self.recordFile.write(' '+content+'\r\n\r\n')
        self.recordFile.flush()
    
        if closeAfterWrite == True:
            self.close()

    def close(self):
        if self.recordFile.closed == False:
            self.recordFile.close()
