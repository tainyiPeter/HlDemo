#ifndef LOGPARSERDEF_H
#define LOGPARSERDEF_H

#include "HighLightDef.h"

#include <QString>
#include <vector>
#include <QDateTime>

/* 动作触发类型
 */
enum class TriggerType
{
    Unknown = 0,
    TrigUser,   //用户操作
    TrigHL,     //抓到高光
};

enum class LineType
{
    Unknow = 0,
    ServiceStart,       //服务重启了之后
    GameStart,          //新开一局游戏
    ImageStart,          //后处理分析开始
};


/** 区域信息
*/
struct AreaInfo
{
    uint32_t m_areaid = 0;
    QString m_strInfo;
};

/* 一帧图片的信息
 */
struct ImageData
{
    //uint32_t  m_cnt = 0;
    GameType  m_gameType = GameType::UNKNOWN;
    std::vector<AreaInfo> m_vecAreaInfo;
};

struct LogData
{
    void SetType(LineType lType)
    {
        if(lType == LineType::ImageStart)
            m_type = TriggerType::TrigHL;
    }
    QDateTime   m_time;
    TriggerType m_type = TriggerType::Unknown;
    ImageData   m_imageData;
};

using LogInfoVec = std::vector<LogData>;

/* 服务次数
 */
struct ServiceTimer
{
    void Reset()
    {
        m_vecData.clear();
        m_timeGap = 0;
    }
    LogInfoVec  m_vecData;
    qint64      m_timeGap = 0;
};

using ServicesVec = std::vector<ServiceTimer>;


#endif // LOGPARSERDEF_H
