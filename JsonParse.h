#ifndef CJSONPARSE_H
#define CJSONPARSE_H

#include <string>
#include <unordered_map>
#include <functional>



class MainWindow;

class CJsonParse
{
public:
    CJsonParse(MainWindow* pParent);

    void DispatchAction(const std::string& strAction, const std::string& strJson);

private:
    void RegisterFunctions();

    void GetHLInfo(const std::string& strJson);

private:
    using FuncClothes = std::function<void(const std::string& strJson)>;
    std::unordered_map<std::string, FuncClothes>		m_mapFuncs;

    MainWindow* m_pMainWindow = nullptr;
};

#endif // CJSONPARSE_H
