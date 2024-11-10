#ifndef CLOGPARSER_H
#define CLOGPARSER_H

#include "HighLightDef.h"
#include "LogParserDef.h"

#include <QMap>
#include <QObject>

class CLogParser :  public QObject
{
    Q_OBJECT
public:
    CLogParser(QObject *parent = nullptr);

signals:
    void logSignal(const LogData&, qint64);
    void ApendMsgSignal(const QString&);

public:
    void TestLog(const QString& strFile);
    void ParseLog(const QString& strFile);
    void ParseLog(const QStringList& strList);
    void Run();

private:

    //解析单行字符串
    LineType ParseLineString(const QString& strLine, LogData& data);
    LineType GetLineType(const QString& strLine);

    //解析时间戳
    QDateTime ParseTimeStamp(const QString& strTime);

    QDateTime ParseServiceStart(const QString& strLine);
    /* 解析后处理字符
     */
    uint32_t ParseImageStart(const QString& strLine);
    AreaInfo ParseImageDetail(const QString& strLine, GameType& gameType);

    /* 取strKey后面的文字，一般如 areaID:64，则返回字符串 64
     */
    QString FetchText(const QString& strSrc, const QString& strKey);
    int FetchTextToDigit(const QString& strSrc, const QString& strKey);
private:
    //LogInfoVec  m_logData;
    ServicesVec  m_Services; //服务重启次数
    // std::unordered_map<std::string, LineType> m_mapLineType;
    QMap<QString, LineType> m_mapLineType;
};

#endif // CLOGPARSER_H
