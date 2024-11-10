#ifndef CJSONACTION_H
#define CJSONACTION_H

#include <QString>
#include <QJsonObject> // { }
#include <QJsonArray> // [ ]
#include <QJsonDocument> // 解析Json
#include <QJsonValue> // int float double bool null { } [ ]
#include <QJsonParseError>

#include <unordered_map>
#include <string>

using ActionMap = std::unordered_map<std::string, QString>;

class CJsonAction
{
public:
    CJsonAction();
    ~CJsonAction();

    static void SaveToDisk(const QString& strFileName, const QString& strJson);
    static bool LoadJson(const QString& strPath, ActionMap& mapJson);

private:
    static ActionMap SetParseByDir(const QString &dirPath);
    static QString LoadJson(const QString& strFile);
};

#endif // CJSONACTION_H
