import binascii
import struct

def BitsStrFromHex(h,Len=4):
    return bin(int(h,16))[2:].zfill(Len)

def HexChrFromFloat(f):
    return binascii.b2a_hex(struct.pack('>f',f))

def Int10FromFloat(f):
    result=HexChrFromFloat(f)
    return int(result[0:4],16),int(result[4:8],16)

def FloatFrom32Bits(Bits,Len=9):
    result=struct.unpack('>f',struct.pack('>I',int(Bits,2)))
    return ('%%0.%df' % Len )%result[0]

def HexChrFromNum(Num):
    return binascii.b2a_hex(struct.pack('>h',Num))

#'3fa51eb8'
def FloatFrom4Hexs(Hexs,Len=2):
    if len(Hexs)!=8 or Hexs==0 or Hexs==None:
        return ''    
    return ('%%0.%df' % Len )%struct.unpack('>f',binascii.unhexlify(Hexs))[0]

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

