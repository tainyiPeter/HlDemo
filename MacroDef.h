#ifndef MACRODEF_H
#define MACRODEF_H

#define         DebugPath               "/Debug"
#define         HlData                  "/HlData"
#define         JsonData                "/jsonData"

#define         JsonFramework           "/FrameWork"
#define         JsonStyle               "/Style"
#define         JsonAutoRecord          "/AutoRecord"
#define         JsonManualRecord        "/ManualRecord"
#define         JsonMyTest              "/MyTest"
#define         JsonVideoDb              "/VideoDb"



#define         ModuleId                2017
#define         JsonFile                ".json"

//Action define
#define         Action_MyTest           "my_test"
#define         Action_Enter            "gamingai_enter"
#define         Action_Exit             "gamingai_exit"

#define         LogFile                 "SEGameHLController.log"

#define         LogStart                "Module:SEGameHLController,version"
#define         LogImage                "post game event, area cnt:"
#define         LogImageDetail          "post detail gametype:"
#define         PostAreaId              "areaID:"
#define         PostText                "info:"
#define         TimeSplit               " : "
#define         CommaSplit              ","
#define         SpaceSplit              " "
#define         CurYear                 "2024"

#define         DataTimeFormat          "yyyy-MM-dd hh:mm:ss"


//json 字符串定义
/*
 * {"type":"post_hl","data":{"gameType":4,"areaInfo":[{"areaId":12,"info":"祝下次好运！"}]}}
 */
#define         JsType            "type"
#define         JsData            "data"
#define         JsGameType        "gameType"
#define         JsAreaInfo        "areaInfo"
#define         JsAreaId          "areaId"
#define         JsInfo            "info"
#define         JsPost_Hl         "post_hl"


#endif // MACRODEF_H
