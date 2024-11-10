#ifndef CHLMANAGER_H
#define CHLMANAGER_H

#include "GuiInterface.h"
#include <QString>

class CHLManager
{
public:
    CHLManager();

    void UpdateHlInfo();
    void SetHLInfo(const HLInfo& info) {m_hlInfo = info;}

    QString GetSavePath() { return m_hlInfo.m_strVideoSavePath;}

private:
    HLInfo          m_hlInfo;
};

#endif // CHLMANAGER_H
