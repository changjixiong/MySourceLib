import binascii
import struct

def BitsStrFromHex(h,Len=4):
    return bin(int(h,16))[2:].zfill(Len)

def HexChrFromFloat(f, endian='>f'):
    return binascii.b2a_hex(struct.pack(endian,f))

def Int10FromFloat(f):
    result=HexChrFromFloat(f)
    return int(result[0:4],16),int(result[4:8],16)

def FloatFrom32Bits(Bits,endianf='>f',endiani='>I',Len=9):
    result=struct.unpack(endianf,struct.pack(endiani,int(Bits,2)))
    return ('%%0.%df' % Len )%result[0]

def HexChrFromNum(Num, endian='>h'):
    return binascii.b2a_hex(struct.pack(endian,Num))

#'3fa51eb8'
def FloatFromHexChr(Hexs, endian='>f'):
    if len(Hexs)!=8 or Hexs==0 or Hexs==None:
        return ''    
    return struct.unpack(endian,binascii.unhexlify(Hexs))[0]

def HexStyleFromString(strSrc):
    return ' '.join(strSrc[i:i+2] for i in xrange(0, len(strSrc),2))

def StringFromHexStyle(strSrc, space=1):
    return ''.join(strSrc[i:i+2] for i in xrange(0, len(strSrc),2+space))

def IntFromStream(data, endian='>h'):
    return struct.unpack(endian, data)[0]
