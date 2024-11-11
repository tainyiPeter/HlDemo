#include "utility.h"
#include "MacroDef.h"
#include <QMessageBox>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

/*
 * json 解析
 * https://blog.csdn.net/a1937194471/article/details/142312542
 */

CUtility::CUtility()
{

}

void CUtility::ShowBox(const QString &msg)
{
    QMessageBox::information(nullptr,
                             "标题",
                             msg,
                             QMessageBox::Ok | QMessageBox::Cancel,
                             QMessageBox::Ok);
}

QString CUtility::GetBinPath()
{
    QString strPath = QDir::currentPath();
    return  strPath;
}

QString CUtility::GetHlDataPath()
{
    QString strPath = QDir::currentPath();
    qDebug()<< "currentPath:" << strPath;
    //strPath += DebugPath;
    strPath += HlData;

    return strPath;
}

QString CUtility::GetJsonDataPath()
{
    QString strPath = GetHlDataPath();
    strPath += JsonData;

    return strPath;
}

QString CUtility::ImageDataToJson(const ImageData& data)
{
    /*
     * {
     *  "type":"post",
     *  "data":{
     *          "gameType":2,
     *          "areaInfo":
     *          [
     *          {"areaId":19,"info":"击/死/助 3/6/0"},
     *          {"areaId":19,"info":"击/死/助 5/4/0"}
     *          ]
     *         }
     * }
     */
     QJsonObject jsonData;
     jsonData.insert(JsGameType, (int)data.m_gameType);

     QJsonArray arrArea;
     for(auto it:data.m_vecAreaInfo)
     {
         QJsonObject  hlData;
         hlData.insert(JsAreaId, (int)it.m_areaid);
         hlData.insert(JsInfo, it.m_strInfo);

         arrArea.push_back(hlData);
     }
     jsonData.insert(JsAreaInfo, arrArea);

     QJsonObject JsonRoot;//=new QJsonObject();
     JsonRoot.insert(JsType, JsPost_Hl);
     JsonRoot.insert(JsData, jsonData);

     //Json to string
     QJsonDocument doc(JsonRoot);
     QByteArray json = doc.toJson();

     return json;

//     JsonObject.insert("age","30");
//     JsonObject.insert("isStudent","false");

//     //courses数组处理
//     QJsonArray array;

//     //courses数组中第一个对象
//     QJsonObject courses1;
//     courses1.insert("courseName","Mathematics");
//     courses1.insert("credits",3);

//     QJsonObject instructor;   //courses1这个对象里面又嵌套了对象，所以也得特殊处理
//     instructor.insert("name","Dr. Smith");
//     instructor.insert("email","smith@example.com");
//     courses1.insert("instructor",instructor);//处理完了进行插入

//     //courses数组中第二个对象
//     QJsonObject courses2;
//     courses2.insert("courseName","Physics");
//     courses2.insert("credits",4);
//     QJsonObject instructor2;
//     instructor2.insert("name","Dr. Johnson");
//     instructor2.insert("email","johnson@example.com");
//     courses2.insert("instructor",instructor2);

//     array.push_back(courses1);//JsonArrary数组插入数据
//     array.push_back(courses2);
//     JsonObject->insert("courses",array);//array里面的成员全部处理完了，插入最外层对象
//     // 当不再需要 JsonObject 时，使用 delete 删除它
//     delete JsonObject;



   // return strJson;

}
