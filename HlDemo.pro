QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR     = $$PWD/../bin

SOURCES += \
    HLManager.cpp \
    JsonAction.cpp \
    JsonParse.cpp \
    LogParser.cpp \
    SEHelper.cpp \
    main.cpp \
    mainwindow.cpp \
    utility.cpp

HEADERS += \
    GuiInterface.h \
    HLManager.h \
    HighLightDef.h \
    JsonAction.h \
    JsonParse.h \
    LogParser.h \
    LogParserDef.h \
    MacroDef.h \
    SEHelper.h \
    mainwindow.h \
    utility.h

FORMS += \
    mainwindow.ui

RC_ICONS = hl.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
