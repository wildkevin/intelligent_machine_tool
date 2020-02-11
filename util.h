#ifndef UTIL_H
#define UTIL_H
#include <QByteArray>
#include <ruleobject.h>
#include <QFile>

namespace Util {
class SerialPortUtil;
class ConfigUtil;
}
class SerialPortUtil{
public:
    static QList<QString> *DeParseReadData(QList<RuleObject*> *rules, QString dataOrigin);
};

class ConfigUtil{
public:
    static QList<RuleObject*>* ParseXMLConfig(QFile *file);
};

#endif //UTIL_H

//char to hex
static inline char CharToHex(char ch)
{
    if((ch>=48)&&(ch<=57))
    {
        ch = (ch-48)+0x00;
    }
    else if((ch>=65)&&(ch<=70))//Capital
    {
        ch = (ch-65)+0x0A;
    }
    else
    {
        ch = 0xff;
    }
    return ch;
}

// hex to ascii
static inline char HexToChar(char hex)
{
    if((hex>=0x30)&&(hex<=0x39))
    {
        hex -= 0x30;
    }
    else if((hex>=0x41)&&(hex<=0x46)) // Capital
    {
        hex -= 0x37;
    }
    else if((hex>=0x61)&&(hex<=0x66)) //littlecase
    {
        hex -= 0x57;
    }
    else
    {
        hex = 0xff;
    }
    return hex;
}


//16进制转换函数
static inline QByteArray QString2Hex(QString str)
{
        QByteArray senddata;
        int hexdata,lowhexdata;
        int hexdatalen = 0;
        int len = str.length();
        senddata.resize(len/2);
        char lstr,hstr;
        for(int i=0; i<len; )
        {
            hstr=str[i].toLatin1();
            if(hstr == ' ')
            {
                i++;
                continue;
            }
            i++;
            if(i >= len)
                break;
            lstr = str[i].toLatin1();
            hexdata = CharToHex(hstr);
            lowhexdata = CharToHex(lstr);
            if((hexdata == 16) || (lowhexdata == 16))
                break;
            else
                hexdata = hexdata*16+lowhexdata;
            i++;
            senddata[hexdatalen] = (char)hexdata;
            hexdatalen++;
        }
        senddata.resize(hexdatalen);
    return senddata;
}
