#ifndef CUTILITY_H
#define CUTILITY_H

#include "LogParser.h"

#include <QString>
#include <QDir>


class CUtility
{
public:
    CUtility();

    static void  ShowBox(const QString& msg);

    static QString GetBinPath();
    static QString GetHlDataPath();
    static QString GetJsonDataPath();

    static QString ImageDataToJson(const ImageData& data);
};

#endif // CUTILITY_H
