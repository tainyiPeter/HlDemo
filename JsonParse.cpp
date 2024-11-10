#include "JsonParse.h"
#include "GuiInterface.h"
#include "mainwindow.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

CJsonParse::CJsonParse(MainWindow* pParent)
    :m_pMainWindow(pParent)
{
    RegisterFunctions();
}

void CJsonParse::DispatchAction(const std::string &strAction, const std::string &strJson)
{
    auto it = m_mapFuncs.find(strAction);
    if(it == m_mapFuncs.end()) return;

    return it->second(strJson);
}

void CJsonParse::RegisterFunctions()
{
    m_mapFuncs =
    {
        {"get_hl_info", std::bind(&CJsonParse::GetHLInfo, this, std::placeholders::_1)},

    };
}

void CJsonParse::GetHLInfo(const std::string& strJson)
{
    if(strJson.empty()) return;

    QByteArray byteArray(strJson.c_str(), strJson.length());

    HLInfo info;
    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(byteArray, &err_rpt); // 字符串格式化为JSON
    if(err_rpt.error != QJsonParseError::NoError)
    {
        return;
    }

    QJsonObject root_Obj = root_Doc.object();
    int error_code = root_Obj.value("error_code").toInt();
    if(error_code != 0)
    {
        qDebug() << "GetHLInfo error_code:" << error_code;
        return;
    }

    QJsonObject jsonData = root_Obj.value("data").toObject();
    info.m_strVideoSavePath = jsonData.value("video_save_path").toString();

    if(nullptr != m_pMainWindow)
    {
        m_pMainWindow->SetHLInfo(info);
    }

    QString strMsg = "save_path";
    strMsg += ":";
    strMsg += info.m_strVideoSavePath;

    m_pMainWindow->AppendMsg(QString("%1:%2").arg("savepath").arg(info.m_strVideoSavePath));
}
