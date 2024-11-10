#include "AgentMgr.h"
#include "utility.h"

#include <QString>
#include <QDebug>


CAgentMgr::CAgentMgr()
{
}

CAgentMgr::~CAgentMgr()
{

}

void CAgentMgr::Init()
{
  // CString path =  _T("SEAgent.dll");

  // QString strPath = "SEAgent.dll";
  // strPath.toWCharArray()

  // std::wstring strL = L"SEAgent.dll";

    hModule = LoadLibrary(L"SEAgent.dll");

    // check dll here
    if(nullptr == hModule)
    {
        CUtility::ShowBox("加载dll失败");
        qDebug() << "加载dll失败" ;
        //wprintf(L"LoadError:%d path:%s", GetLastError(), path);
        return;
    }

    se_syn_register = (fn_se_syn_register)GetProcAddress(hModule, "se_syn_register");
    if (se_syn_register == NULL)
    {
        int e = GetLastError();
        wprintf(L"Fail to load se_syn_register(),error code is %d\n", e);
        return;
    }

    se_syn_send = (fn_se_syn_send)GetProcAddress(hModule, "se_syn_send");
    if (se_syn_send == NULL)
    {
        int e = GetLastError();
        wprintf(L"Fail to load se_syn_send(),error code is %d\n", e);
        return -1;
    }

    se_msg_close = (fn_se_msg_close)GetProcAddress(hModule, "se_msg_close");
    if (se_msg_close == NULL)
    {
        int e = GetLastError();
        printf("Fail to load se_msg_close(),error code is %d\n", e);
        return -1;
    }

    // If in consolue mode , you could break the process by get char
    // getchar();

    // Init the new interface for PCM , 20 is the init code here
    int nret = se_syn_register(20, SyncChrCallBack, SyncChrDelResp);

//    char buffer[1024*10];
//    int n =se_syn_send(1, "ability", NULL, buffer, 1024 * 10);

//    getchar();

    CUtility::ShowBox("初始化成功");
}
