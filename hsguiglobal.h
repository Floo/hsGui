#ifndef HSGUIGLOBAL_H
#define HSGUIGLOBAL_H

#include <QtCore>

struct confElement
{
    QObject *object;
    QString section;
    QString key;
    bool boolValue;
    int intValue;
    QString strValue;
    bool isBool;
    bool isInt;
    bool isTime;
    bool hasChanged;
};

struct confLueftung
{
    bool bHWRTemp;
    bool bHWRDauer;
    bool bACLow;
    int iHWRSchelle;
};

#endif // HSGUIGLOBAL_H
