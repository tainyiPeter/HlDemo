#ifndef CAGENTMGR_H
#define CAGENTMGR_H

#include <windows.h>


typedef void(__cdecl* fnSyncChrCallBack)(const char* action, const char* data, char** resp);
typedef void(__cdecl* fnSyncChrDelResp)(char* resp);


typedef int (*fn_se_syn_register) (int module, fnSyncChrCallBack callback, fnSyncChrDelResp delresp);
typedef int (*fn_se_syn_send)(int module, const char* action, const char* paramter, char* response, int length);

typedef int (*fn_se_msg_close)();


class CAgentMgr
{
public:
    CAgentMgr();
    ~CAgentMgr();

    static CAgentMgr* Instance()
    {
        static CAgentMgr __Instance;
        return &__Instance;
    }

    void Init();
private:

    HMODULE         hModule = nullptr;

    fn_se_syn_register se_syn_register = nullptr;
    fn_se_syn_send se_syn_send = nullptr;
    fn_se_msg_close se_msg_close = nullptr;
    fnSyncChrCallBack g_chrcallback = nullptr;

};

#endif // CAGENTMGR_H
