import binascii
import struct

def HexChrfromNum(num,form):
    return struct.pack(form,num)

def NumforHexChr(HexChr):
    if HexChr is None:
        return 0
    if len(HexChr)==2:
        return  (struct.unpack('<H',HexChr))[0]
    if len(HexChr)==4:
        return (struct.unpack('<I',HexChr))[0]
    else:
        return 0

def printInfo(filename):
    f=open(filename,'rb')
    data=f.read()   
    f.close()
    Identifier=data[:2]
    FileSize=data[2:6]
    Reserved=data[6:10]
    BitmapDataOffset=data[10:14]
    BitmapHeaderSize=data[14:18]
    Width=data[18:22]
    Height=data[22:26]
    Planes=data[26:28]
    BitsPerPixel=data[28:30]
    Compression=data[30:34]
    BitmapDataSize=data[34:38]
    HResolution=data[38:42]
    VResolution=data[42:46]
    Colors=data[46:50]
    ImportantColors=data[50:54]
    if BitsPerPixel==8:
        Palette=data[54:1078]
        BitmapData=data[1078:]
    else:
        BitmapData=data[54:]

    print 'Identifier',Identifier
    print 'FileSize',NumforHexChr(FileSize)
    print 'Reserved',NumforHexChr(Reserved)
    print 'BitmapDataOffset',NumforHexChr(BitmapDataOffset)
    print 'BitmapHeaderSize',NumforHexChr(BitmapHeaderSize)
    print 'Width',NumforHexChr(Width)
    print 'Height',NumforHexChr(Height)
    print 'Planes',NumforHexChr(Planes)
    print 'BitsPerPixel',NumforHexChr(BitsPerPixel)
    print 'Compression',NumforHexChr(Compression)
    print 'BitmapDataSize',NumforHexChr(BitmapDataSize)
    print 'HResolution',NumforHexChr(HResolution)
    print 'VResolution',NumforHexChr(VResolution)
    print 'Colors',NumforHexChr(Colors)
    print 'ImportantColors',NumforHexChr(ImportantColors)
    if BitsPerPixel==8:
        for i in range(0,1024,4):            
            print 'Palette',i/4,binascii.b2a_hex(Palette[i:i+4])
    print 'BitmapData',binascii.b2a_hex(BitmapData)

def convert(filesourc,filedest):
    f=open(filesourc,'rb')
    data=f.read()   
    f.close()
    Identifier=data[:2]
    FileSize=data[2:6]
    Reserved=data[6:10]
    BitmapDataOffset=data[10:14]
    BitmapHeaderSize=data[14:18]
    Width=data[18:22]
    Height=data[22:26]
    Planes=data[26:28]
    BitsPerPixel=data[28:30]
    Compression=data[30:34]
    BitmapDataSize=data[34:38]
    HResolution=data[38:42]
    VResolution=data[42:46]
    Colors=data[46:50]
    ImportantColors=data[50:54]
    
    Palette=data[54:1078]
    BitmapData=data[1078:]

    dicPalette=dict()
    for i in range(256):
        dicPalette[i]=Palette[i<<2:(i<<2)+3]
    #print dicPalette
    index=0
    BitmapData24=''

    W=NumforHexChr(Width)
    H=NumforHexChr(Height)
    addLen= 4 - (W*3)%4
    if addLen==4:
        addLen=0

    print addLen
    for i in range(H):
        for j in range(W):
            BitmapData24+=dicPalette[ord(BitmapData[index+j])]
        BitmapData24+='\x00'*addLen
        index+=((W+(4-1))/4)*4

    BitmapDataOffset =NumforHexChr(BitmapDataOffset) - 1024
    BitmapDataSize=len(BitmapData24)
    FileSize=BitmapDataOffset+BitmapDataSize
    BitsPerPixel=24
    
    BitmapDataOffset=HexChrfromNum(BitmapDataOffset,'<I')
    BitmapDataSize=HexChrfromNum(BitmapDataSize,'<I')    
    FileSize=HexChrfromNum(FileSize,'<I')
    BitsPerPixel=HexChrfromNum(BitsPerPixel,'<H')

    f=open(filedest,'wb')
    f.write(Identifier)
    f.write(FileSize)
    f.write(Reserved)
    f.write(BitmapDataOffset)
    f.write(BitmapHeaderSize)
    f.write(Width)
    f.write(Height)
    f.write(Planes)
    f.write(BitsPerPixel)
    f.write(Compression)
    f.write(BitmapDataSize)
    f.write(HResolution)
    f.write(VResolution)
    f.write(Colors)
    f.write(ImportantColors)
    f.write(BitmapData24)
    f.close()    

#printInfo('24.bmp')
#convert('8.bmp','24.bmp')

