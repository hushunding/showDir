#-------------------------------------------------
#
# Project created by QtCreator 2013-10-27T23:24:21
#
#-------------------------------------------------

QT       += core gui  sql
greaterThan(QT_VERSION, 5.6){
    DEFINES += HAS_WEBVIEW
    QT+= webenginewidgets

}
else
{
}
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



TARGET = ShowDir
TEMPLATE = app
#message($$CONFIG)

SOURCES += \
    showdir.cpp \
    mylistview.cpp \
    main.cpp \
    mypiclabel.cpp \
    mypiclist.cpp \
    myscrollarea.cpp

HEADERS  += \
    showdir.h \
    mylistview.h \
    mypiclabel.h \
    mypiclist.h \
    myscrollarea.h

FORMS    += \
    showdir.ui \
    mypiclist.ui

RESOURCES += \
    showdir.qrc

#DEFINES += QT_NO_DEBUG_OUTPUT

