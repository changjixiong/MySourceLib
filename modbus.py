from crc16 import CRC16
import binascii

def ModbusRTUCMD(deviceID, codeNo, fromReg, regNum):
    strCmd = ''.join(map(chr,
                             (deviceID, codeNo,
                              fromReg>>8, fromReg&0xff,
                              regNum>>8, regNum&0xff)))

    crc = (CRC16(strCmd))
    strCmd+=chr(crc>>8)
    strCmd+=chr(crc&0xff)
    
    strCmdAscii = binascii.b2a_hex(strCmd)
    #print ' '.join(strCmdAscii[i:i+2] for i in range(0, len(strCmdAscii), 2))

    return strCmd
    

def ModbusTCPCMD(deviceID,cmdType,FromReg,*args):
    if len(args)==0:
        print 'len is 0'
        return ''

    registerOffSet = 1
    cmdString=''.join(map(chr,(deviceID,cmdType,((FromReg - registerOffSet)>>8),(FromReg - registerOffSet)&0xff)\
                               +(args[0]>>8,args[0]&0xff)))

    #print cmdString
    if len(args)>1:
        cmdString+=''.join(map(chr,(args[0]*2,)\
                               +reduce(tuple.__add__, [(value>>8,value&0xff) for value in args[1:]])))
    return '\x00\x00\x00\x00\x00'+chr(len(cmdString))+cmdString
    
