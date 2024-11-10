#include "JsonAction.h"
#include "MacroDef.h"
#include "utility.h"
#include <QDir>
#include <QFile>
#include <QtDebug>

CJsonAction::CJsonAction()
{
  //  Init();
}

CJsonAction::~CJsonAction()
{

}

ActionMap CJsonAction::SetParseByDir(const QString &dirPath)
{
    std::unordered_map<std::string, QString>  mapJson;

    QStringList fileList;
    QDir dir(dirPath);
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Files | QDir::NoDotAndDotDot | QDir::Dirs);
    foreach (auto fileInfo, fileInfoList)
    {
        if(fileInfo.isDir())
        {
            SetParseByDir(fileInfo.absoluteFilePath());
        }

        if(fileInfo.isFile())
        {
            qDebug() << __FUNCTION__ << __LINE__ << "  : " << fileInfo.absoluteFilePath();
            std::string strFile = fileInfo.baseName().toStdString();
            mapJson[strFile] = LoadJson(fileInfo.absoluteFilePath());
        }
    }

    return mapJson;
}

QString CJsonAction::LoadJson(const QString& strFile)
{
    QFile file(strFile);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "can't open error!";
        return "";
    }

    // 读取文件的全部内容
    QTextStream stream(&file);
    stream.setCodec("UTF-8");		// 设置读取编码是UTF8
    QString str = stream.readAll();
    file.close();

    return str;
}

void CJsonAction::SaveToDisk(const QString &strFileName, const QString &strJson)
{
    QFile file(strFileName);
    QTextStream stream(&file);
    stream.setCodec("UTF-8");		// 设置读取编码是UTF8
    file.open(QIODevice::WriteOnly | QIODevice::Truncate); //只写 与 添加 打开

    stream << strJson;
    stream.flush();
    file.close();
}

bool CJsonAction::LoadJson(const QString& strPath, ActionMap& mapJson)
{
    mapJson = SetParseByDir(strPath);

    return true;
}
