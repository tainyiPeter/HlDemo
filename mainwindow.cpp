#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utility.h"
#include "MacroDef.h"

#include <qt_windows.h>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QProcess>
#include <QThread>
#include <QRandomGenerator>

#include <QJsonObject > // { }
#include <QJsonArray > // [ ]
#include <QJsonDocument > // 解析Json
#include <QJsonValue > // int float double bool null { } [ ]
#include <QJsonParseError>

#ifdef _MSC_VER
 #pragma comment(lib, "user32.lib")
#endif

UINT WM_USER_CALLBACK = WM_USER + 1;


/*
* SE回调
*/
void TestSyncChrCallBack(const char* action, const char* data, char** resp)
{
    HWND hMainWindow = (HWND)MainWindow::GetInstance()->winId();
    if(NULL == hMainWindow) return;
//    qDebug() << "-----------------t1------------------------";
//    qDebug() << data ;
//    qDebug() << "len:"<< strlen(data);
//    qDebug() << "-----------------t2------------------------";

    // data
    int len = strlen(data);
    if(len == 0) return;
    len += 1;
    char* pNewData = new char[len];
    strcpy_s(pNewData, len, data);

    // action
    len = strlen(action);
    len += 1;
    char* pNewAction = new char[len];
    strcpy_s(pNewAction, len, action);

    //PostMessage(hMainWindow, WM_USER_CALLBACK, WPARAM(pNewData), LPARAM(len));
    PostMessage(hMainWindow, WM_USER_CALLBACK, WPARAM(pNewAction), LPARAM(pNewData));
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_jsonParser(this)
{
    ui->setupUi(this);

    //设置按钮信号-槽
    connect(ui->pbInit, SIGNAL(clicked()), this, SLOT(OnInit()));
    connect(ui->pbAction, SIGNAL(clicked()), this, SLOT(OnAction()));
    connect(ui->pbLogAction, SIGNAL(clicked()), this, SLOT(OnLogAction()));
    connect(ui->pbTest, SIGNAL(clicked()), this, SLOT(OnTestClick()));
    connect(ui->pbHL, SIGNAL(clicked()), this, SLOT(OnHL()));
    connect(ui->cbAction, SIGNAL(activated(const QString&)), this , SLOT(OnActiveChange(const QString&)));
    connect(ui->pbClearMsg, SIGNAL(clicked(bool)), this, SLOT(OnClearMsgClick()));

    //设置单选框信号-槽
    connect(ui->rbFramework, SIGNAL(clicked(bool)), this, SLOT(OnRbFramework()));
    connect(ui->rbStyle, SIGNAL(clicked(bool)), this, SLOT(OnRbFramework()));
    connect(ui->rbAutoRecord, SIGNAL(clicked(bool)), this, SLOT(OnRbFramework()));
    connect(ui->rbManualRecord, SIGNAL(clicked(bool)), this, SLOT(OnRbFramework()));
    connect(ui->rbMyTest, SIGNAL(clicked(bool)), this, SLOT(OnRbMyTest()));
    connect(ui->rbVideoDb, SIGNAL(clicked(bool)), this, SLOT(OnRbVideoDb()));

    //connect(&m_logParser, &CLogParser::logSignal(LogData&), this, &MainWindow::handleLogSignal(LogData&));
    //自定义信号-槽
    connect(&m_logParser, SIGNAL(logSignal(const LogData&, qint64)), this, SLOT(handleLogSignal(const LogData&, qint64)));
    connect(&m_logParser, SIGNAL(ApendMsgSignal(const QString&)), this, SLOT(handleApendMsg(const QString&)));

    setAcceptDrops(true);

    Init();
}

void MainWindow::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == m_timerId)
    {

    }
}

MainWindow::~MainWindow()
{
    //DoAction(Action_Exit, "");
    delete ui;
}

void MainWindow::Init()
{
    // 初始化随机数生成器
    //QRandomGenerator::seed(QRandomGenerator::);

    if(-1 == seinvoke_init(TestSyncChrCallBack))
    {
        qDebug("seinvoke_init failed");
        return;
    }

    LoadData();
    m_pRadioGroup = new QButtonGroup(this);
    m_pRadioGroup->addButton(ui->rbFramework,0);
    m_pRadioGroup->addButton(ui->rbStyle,1);
    m_pRadioGroup->addButton(ui->rbAutoRecord,2);
    m_pRadioGroup->addButton(ui->rbManualRecord,3);
    m_pRadioGroup->addButton(ui->rbMyTest,4);
    m_pRadioGroup->addButton(ui->rbVideoDb, 5);

    //WrapMode
    ui->edtMsg->setWordWrapMode(QTextOption::WrapMode::NoWrap); //  # 禁止自动换行

    FreshActionList();
    DoAction(Action_Enter, "");
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    if (eventType == "windows_generic_MSG")
    {
       PMSG msg = (PMSG)message;
       if (msg->message == WM_USER_CALLBACK)
       {
           char* pAction = (char*)msg->wParam;
           char* pData = (char*)msg->lParam;
           if(nullptr == pData)
           {
               qDebug() << "antive event failed ..., pChar is null";
               return QWidget::nativeEvent(eventType, message, result);
           }

           QString strMsg = QString(pAction) + "->" + QString(pData);
           AppendMsg(strMsg);
           delete []pAction;
           delete []pData;
       }
    }

    return QWidget::nativeEvent(eventType, message, result);
}

void MainWindow::LoadData()
{
    QString strHLDat = CUtility::GetHlDataPath() + "/data/hl.dat";
    QFile file(strHLDat);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "can't open error!";
        return;
    }

    // 读取文件的全部内容
    QTextStream stream(&file);
    stream.setCodec("UTF-8");		// 设置读取编码是UTF8
    QString str = stream.readAll();
    file.close();
}

void MainWindow::FreshActionList()
{
    QString strPath = GetRadioPath();
    if(strPath.isEmpty())
    {
        AppendMsg("Fresh action list failed");
        return;
    }

    ui->cbAction->clear();
    CJsonAction::LoadJson(strPath, m_mapAction);
    for(auto it : m_mapAction)
    {
        QString strItem = QString::fromStdString(it.first);
        ui->cbAction->addItem(strItem);
    }
}

void MainWindow::Output(const QString &strInfo)
{
    qDebug() << strInfo;
    ui->edtOutput->setText(strInfo);
}

void MainWindow::UpdateAction(const std::string &strCbSelected, const QString &strJson)
{
    auto it = m_mapAction.find(strCbSelected);
    if(it == m_mapAction.end())
    {
        Output("update action failed");
        return;
    }

    it->second = strJson;
}

bool MainWindow::CheckValidJsonData(const QString &strJson)
{
    if(strJson.isEmpty()) return true;

    QJsonParseError err_rpt;
    QJsonDocument root_Doc = QJsonDocument::fromJson(strJson.toUtf8(), &err_rpt); // 字符串格式化为JSON

    if(err_rpt.error != QJsonParseError::NoError)
    {
        Output("JSON格式错误");
        return false;
    }

    return true;
}

void MainWindow::AppendMsg(const QString &strMsg)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString strTime = currentTime.toString(DataTimeFormat);
    QString strInfo = strTime + ":   " + strMsg;

    ui->edtMsg->append(strInfo);
}

void MainWindow::DoAction(const std::string &strAction, const std::string& strJson)
{
    // do action
    char szResponse[1024] = { 0 };
    int ret = seinvoke_send(ModuleId, strAction.c_str(), strJson.c_str(), szResponse, 1024);

    QString strRsp = QString::fromStdString(szResponse);
    QString strOutput = "ret:" + QString::number(ret) +  "\nrsp:" + strRsp;
    Output(strOutput);

    m_jsonParser.DispatchAction(strAction, szResponse);
}

//打开explorer
void MainWindow::OnInit()
{
    QString strPath = GetSavePath();
    OpenFileInExplorer(strPath);
}

bool MainWindow::IsMyTestRadio()
{
    if(nullptr == m_pRadioGroup) return false;

    return m_pRadioGroup->checkedId() == 4;
}

QString MainWindow::GetRadioPath()
{
    if(nullptr == m_pRadioGroup) return "";
    QString strPath = CUtility::GetJsonDataPath();
    switch (m_pRadioGroup->checkedId())
    {
    case 0:
        strPath += JsonFramework;
        break;
    case 1:
        strPath += JsonStyle;
        break;
    case 2:
        strPath += JsonAutoRecord;
        break;
    case 3:
        strPath += JsonManualRecord;
        break;
    case 4:
        strPath += JsonMyTest;
        break;
    case 5:
        strPath += JsonVideoDb;
        break;
    default:
        return "";
    }

    return strPath;
}

void MainWindow::OpenFileInExplorer(const QString &strPath)
{
    QStringList args;
    //args << "/select," << QDir::toNativeSeparators(strPath);
    args << "/open," << QDir::toNativeSeparators(strPath);

    QProcess *process = new QProcess(this);
    process->start("explorer.exe", args);
}

void MainWindow::OnAction()
{
    std::string strAction = ui->cbAction->currentText().toStdString();
    QString strParam = ui->edtParam->toPlainText();

    QString strLine = ui->leAction->text();
    if(!strLine.isEmpty())
    {
        //手动输入参数
        strAction = strLine.toStdString();
        AppendMsg("manual action");
    }
    else
    {
        //使用组合框
        CJsonAction::SaveToDisk(m_strSelectedFile, strParam);
        UpdateAction(strAction, strParam);
        if(IsMyTestRadio()) strAction = Action_MyTest;
    }

    DoAction(strAction, strParam.toStdString());
}

void MainWindow::OnLogAction()
{
    AppendMsg("do nothing");

    return;

    QString strPath = QDir::currentPath();
    //strPath += DebugPath;
    strPath += "/Log";
    QString strFile = strPath + "/" + LogFile;
    m_logParser.ParseLog(strFile);

    AppendMsg("run ...");
    m_logParser.Run();
    AppendMsg("Finish running ...");
}

void MainWindow::OnHL()
{
    QString text = ui->edtParam->toPlainText();
    QStringList lines = text.split("\n");

    m_logParser.ParseLog(lines);
    AppendMsg("run ...");
    m_logParser.Run();
    AppendMsg("Finish running ...");
}

void MainWindow::OnTestClick()
{

//    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();

//    AppendMsg("currentTime:" + QString::number(timestamp));
//    QString strTime = "2024-11-04 10:50:40:487";
//    //data.m_time = QDateTime::fromString(strTime, "yyyy-MM-dd hh:mm:ss:zzz");
//    QDateTime t =  QDateTime::fromString(strTime, "yyyy-MM-dd hh:mm:ss:zzz");

//    qint64 oldTime = t.toMSecsSinceEpoch();
//    //uint64_t oldTime = t.toSecsSinceEpoch();
//    AppendMsg("oldTime:" + QString::number(oldTime));

//    AppendMsg("abc");
//    AppendMsg("def");

//    m_logParser.TestLog("d:/1.log");


    ui->label_status->setText("wait, " + QString::number(rand()));
   // ui->label_status->show();
}

void MainWindow::OnRbFramework()
{
    FreshActionList();
}

void MainWindow::OnRbStyle()
{
    FreshActionList();
}

void MainWindow::OnRbAutoRecord()
{
    FreshActionList();
}

void MainWindow::OnRbManualRecord()
{
    FreshActionList();
}

void MainWindow::OnRbMyTest()
{
    FreshActionList();
}

void MainWindow::OnRbVideoDb()
{
    FreshActionList();
}

void MainWindow::OnClearMsgClick()
{
    ui->edtMsg->setText("");
    ui->edtOutput->setText("");
}
void MainWindow::OnActiveChange(const QString& text)
{
    //QVariant var = ui.comboBox->itemData(index);
    // 访问用户自定义数据
    //qDebug() << var.value<MyType>().x << endl;
    //qDebug() << ui.comboBox->currentText() << endl;
    // qDebug() << text << endl;

    std::string strKey = text.toStdString();
    auto it = m_mapAction.find(strKey);
    if(it == m_mapAction.end())
    {
        CUtility::ShowBox("not find");
        return;
    }
    m_strSelectedFile = GetRadioPath() + "/" + text + JsonFile;
    ui->edtParam->setText(it->second);
    ui->leAction->setText("");
}

void MainWindow::handleLogSignal(const LogData& data, qint64 qGap)
{
    AppendMsg("handle log signal, qGap:" + QString::number(qGap) + " curTime:"+QString::number(QDateTime::currentMSecsSinceEpoch()) \
              + " date time:" + QString::number(data.m_time.toMSecsSinceEpoch()));
    while (1) {
        qint64 k = QDateTime::currentMSecsSinceEpoch() - data.m_time.toMSecsSinceEpoch();
        if((QDateTime::currentMSecsSinceEpoch() - data.m_time.toMSecsSinceEpoch()) >qGap)
        {
            AppendMsg("k is:" + QString::number(k) + " date time:" + QString::number(data.m_time.toMSecsSinceEpoch()));
            break;
        }
        ui->label_status->setText("wait: " + QString::number(rand()));
        QThread::msleep(200);
    }
    if(data.m_type == TriggerType::TrigHL)
    {
        //foreach(data.m_imageData.)
        QString strJson = CUtility::ImageDataToJson(data.m_imageData);
        AppendMsg("-----------------json start--------------------------------------");
        AppendMsg(strJson);
        AppendMsg("------------------json end --------------------------------------");
        DoAction(Action_MyTest, strJson.toStdString());
    }
    AppendMsg("Received signal with value, oper type:" + QString::number((int)data.m_type));
}

void MainWindow::handleApendMsg(const QString &strMsg)
{
    AppendMsg(strMsg);
}

void MainWindow::handleTest(const QString &strMsg)
{
    AppendMsg("hadle test");
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
    {
        event->acceptProposedAction();
        qDebug()<< "a ... " ;
    }
    else
    {
        qDebug() << "b ...";
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
    {
        Output("url is empty");
        return;
    }

    QString fileName = urls.first().toLocalFile();
    if(fileName.isEmpty())
    {
        Output("fileName is empty");
        return ;
    }

    if(m_timerId != 0)
    {
        killTimer(m_timerId);
    }

   // m_timerId = startTimer(1000);

    AppendMsg(tr("parse file:%1").arg(fileName));
    m_logParser.ParseLog(fileName);
    AppendMsg("finish parse");

   // return;

    m_logParser.Run();
    AppendMsg("Finish running ...");
}
