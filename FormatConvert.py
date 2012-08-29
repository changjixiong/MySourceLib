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


