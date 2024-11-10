#include "LogParser.h"
#include "MacroDef.h"
#include <QFile>
#include <QDebug>
#include <QThread>

#define     LogFileName     "SEGameHL.log"

CLogParser::CLogParser(QObject *parent)
    :QObject(parent)
{
    m_mapLineType =
    {
        { LogStart,   LineType::ServiceStart },
        { LogImage,   LineType::ImageStart },
    };
}

void CLogParser::TestLog(const QString &strFile)
{
    QStringList strList;
    QFile file(strFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QByteArray line = file.readLine().trimmed(); //即可去除行尾换行符空格等。
            strList.append(line);

           // QString strLine(line);
        }
    }

    QString strInfo = "cnt:"+ QString::number(strList.size());
    emit ApendMsgSignal(strInfo);
   // return;

    for (QStringList::const_iterator it = strList.constBegin();
         it != strList.constEnd();
         ++it)
    {
       emit ApendMsgSignal(*it);
    }
}

void CLogParser::ParseLog(const QString &strFile)
{
    QStringList strList;
    QFile file(strFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QByteArray line = file.readLine().trimmed(); //即可去除行尾换行符空格等。
            strList.append(line);
        }
    }
    return ParseLog(strList);

//    ServiceTimer st;
//    QFile file(strFile);
//    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
//    {
//        while (!file.atEnd())
//        {
//            QByteArray line = file.readLine();
//            QString strLine(line);
//            LogData info;

//            LineType lType = ParseLineString(strLine, info);
//            if(LineType::ServiceStart == lType)
//            {
//                if(st.m_timeGap > 0)
//                {
//                    m_Services.push_back(st);
//                    st.Reset();
//                }
//                st.m_timeGap = QDateTime::currentMSecsSinceEpoch() - info.m_time.toMSecsSinceEpoch();
//                st.m_vecData.push_back(info);
//            }
//            else if(LineType::ImageStart == lType)
//            {
//                uint32_t cnt = ParseImageStart(strLine);
//                GameType gType = GameType::UNKNOWN;
//                for(uint32_t idx = 0; idx < cnt; ++idx)
//                {
//                    if(file.atEnd())
//                        break;
//                    line = file.readLine();
//                    strLine = QString(line);
//                    AreaInfo vi = ParseImageDetail(line, gType);
//                    if(vi.m_areaid > 0)
//                    {
//                        info.m_imageData.m_vecAreaInfo.push_back(vi);
//                    }
//                }

//                info.m_imageData.m_gameType = gType;

//                if(info.m_imageData.m_gameType != GameType::UNKNOWN)
//                {
//                    info.SetType(lType);
//                    st.m_vecData.push_back(info);
//                }
//            }
//        }

//        file.close();
//    }

//    if(st.m_timeGap > 0)
//    {
//        m_Services.push_back(st);
//    }
}

void CLogParser::ParseLog(const QStringList &strList)
{
    if(strList.empty())
    {
        emit ApendMsgSignal("ParseLog failed");
        return;
    }

    ServiceTimer st;
    for (QStringList::const_iterator it = strList.constBegin();
         it != strList.constEnd();
         ++it)
    {
        QString strLine = *it;
        LogData info;

        LineType lType = ParseLineString(strLine, info);
        if(LineType::ServiceStart == lType)
        {
            if(st.m_timeGap > 0)
            {
                m_Services.push_back(st);
                st.Reset();
            }
            st.m_timeGap = QDateTime::currentMSecsSinceEpoch() - info.m_time.toMSecsSinceEpoch();
            qint64 logTimeStamp = info.m_time.toMSecsSinceEpoch();
            st.m_vecData.push_back(info);
        }
        else if(LineType::ImageStart == lType)
        {
            uint32_t cnt = ParseImageStart(strLine);
            GameType gType = GameType::UNKNOWN;
            for(uint32_t idx = 0; idx < cnt; ++idx)
            {
                it++;
                if(it == strList.end())
                    break;
                strLine = *it;
                AreaInfo vi = ParseImageDetail(strLine, gType);
                if(vi.m_areaid > 0)
                {
                    info.m_imageData.m_vecAreaInfo.push_back(vi);
                }
            }

            info.m_imageData.m_gameType = gType;
            if(info.m_imageData.m_gameType != GameType::UNKNOWN)
            {
                info.SetType(lType);
                st.m_vecData.push_back(info);
            }
        }
    }

    if(st.m_timeGap > 0)
    {
        m_Services.push_back(st);
    }

}

void CLogParser::Run()
{
    for(auto& itSvr : m_Services)
    {
        for(auto& it : itSvr.m_vecData)
        {
            emit logSignal(it, itSvr.m_timeGap);
        }
    }
}

LineType CLogParser::ParseLineString(const QString &strLine, LogData& data)
{
    QStringList strList = strLine.split(TimeSplit);
    if(strList.size() < 2)
        return LineType::Unknow;

    //parse time
    data.m_time = ParseTimeStamp(strList[0]);

    return GetLineType(strList[1]);
}

LineType CLogParser::GetLineType(const QString &strLine)
{
    if(strLine.isEmpty()) return LineType::Unknow;

    for(auto& it : m_mapLineType.toStdMap())
    {
        if(strLine.indexOf(it.first) >= 0)
        {
            return it.second;
        }
    }

    return LineType::Unknow;
}

QDateTime CLogParser::ParseTimeStamp(const QString& strTime)
{
    QString strDateTime = CurYear + QString("-") + strTime;
    QDateTime dt = QDateTime::fromString(strDateTime, "yyyy-MM-dd hh:mm:ss:zzz");

    return dt;
}

QDateTime CLogParser::ParseServiceStart(const QString &strLine)
{
    return ParseTimeStamp(strLine);
}

uint32_t CLogParser::ParseImageStart(const QString& strLine)
{
    int index = strLine.indexOf(LogImage);
    if(index < 0) return 0;
    QString strText = strLine.mid(index+strlen(LogImage));
    QStringList strList = strText.split(CommaSplit);
    if(strList.size() <2) return 0;

    QString strCnt = strList[0];
    return strCnt.toInt();
}

AreaInfo CLogParser::ParseImageDetail(const QString &strLine, GameType& gameType)
{
    AreaInfo ai;

    QStringList strList = strLine.split(CommaSplit);
    if(strList.size() < 3) return ai;
    uint32_t gType = FetchTextToDigit(strList[0], LogImageDetail);
    gameType = GameType(gType);
    ai.m_areaid = FetchTextToDigit(strList[1], PostAreaId);
    ai.m_strInfo = FetchText(strList[2], PostText);

    return ai;
}

QString CLogParser::FetchText(const QString& strSrc,const QString &strKey)
{
    int index = strSrc.indexOf(strKey);
    if(index < 0) return "";
    return strSrc.mid(index+strKey.length());
}

int CLogParser::FetchTextToDigit(const QString &strSrc, const QString &strKey)
{
    QString strText = FetchText(strSrc, strKey);
    return strText.toInt();
}
