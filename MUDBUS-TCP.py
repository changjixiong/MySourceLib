registerOffSet=1
def ModBusTCPCMD(deviceID,cmdType,FromReg,*args):
    if len(args)==0:
        print 'len is 0'
        return ''

    cmdString=''.join(map(chr,(deviceID,cmdType,((FromReg - registerOffSet)>>8),(FromReg - registerOffSet)&0xff)\
                               +(args[0]>>8,args[0]&0xff)))
    if len(args)>1:
        cmdString+=''.join(map(chr,(args[0]*2,)\
                               +reduce(tuple.__add__, [(value>>8,value&0xff) for value in args[1:]])))
    return '\x00\x00\x00\x00\x00'+chr(len(cmdString))+cmdString

