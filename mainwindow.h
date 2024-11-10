#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QButtonGroup>

#include <QString>
#include "SEHelper.h"
#include "GuiInterface.h"
#include "JsonAction.h"
#include "JsonParse.h"
#include "HLManager.h"
#include "LogParserDef.h"
#include "LogParser.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    ~MainWindow();

    bool nativeEvent(const QByteArray &eventType, void *message, long *result);

    static MainWindow* GetInstance()
    {
        static MainWindow __Instance;
        return &__Instance;
    }

    void Output(const QString& strInfo);
    void SetHLInfo(const HLInfo& info) {m_hlMgr.SetHLInfo(info);}
    void AppendMsg(const QString& strMsg);

private:
    MainWindow(QWidget *parent = nullptr);

    void timerEvent(QTimerEvent* event);

    void Init();
    void LoadData();
    void FreshActionList();    
    void UpdateAction(const std::string& strAction, const QString& strJson);
    bool CheckValidJsonData(const QString& strJson);

    void DoAction(const std::string& strAction, const std::string& strJson);
    bool IsMyTestRadio();
    QString GetRadioPath();
    void OpenFileInExplorer(const QString& strPath);
    QString GetSavePath() {return m_hlMgr.GetSavePath();}

protected:
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

public slots:
    void OnInit();
    void OnAction();
    void OnLogAction();
    void OnTestClick();
    void OnHL();

    void OnRbFramework();
    void OnRbStyle();
    void OnRbAutoRecord();
    void OnRbManualRecord();
    void OnRbMyTest();
    void OnRbVideoDb();

    void OnActiveChange (const QString & text );
    void OnClearMsgClick();
    void handleLogSignal(const LogData& data, qint64 qGap);
    void handleApendMsg(const QString& strMsg);

    void handleTest(const QString& strMsg);

private:
    Ui::MainWindow *ui;

    fnSyncChrCallBack   m_callBack = nullptr;
    CLogParser      m_logParser;
    ActionMap       m_mapAction;
    CJsonParse      m_jsonParser;
    CHLManager      m_hlMgr;
    QButtonGroup*   m_pRadioGroup = nullptr;
    QString         m_strSelectedFile;

    int             m_timerId = 0;
    //Test variate
    bool            m_bHLEnable = false;
};


#endif // MAINWINDOW_H

