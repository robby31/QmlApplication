#-------------------------------------------------
#
# Project created by QtCreator 2014-10-22T13:15:24
#
#-------------------------------------------------

TEMPLATE = lib

TARGET = QmlApplication
TARGET = $$qtLibraryTarget($$TARGET)

QT       += qml quick widgets sql
QT       -= gui

qtHaveModule(webview) {
    QT += webview

    contains( QT_MAJOR_VERSION, 5 ) {
      greaterThan( QT_MINOR_VERSION, 6 ) {
         DEFINES += WEBVIEW_PACKAGE
      } else {
         DEFINES += WEBENGINE_PACKAGE
      }
   } else {
      DEFINES += WEBENGINE_PACKAGE
   }
}


!exists($$(MYLIBRARY)) {
    error("variable MYLIBRARY not set.")
}

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

CONFIG += c++11

CONFIG += staticlib

SOURCES +=  \
            Models/listmodel.cpp \
            Models/program.cpp \
            application.cpp \
            UIController/controller.cpp \
            UIController/uiservices.cpp \
            Worker/worker.cpp \
    IO/csvdatatable.cpp \
    Models/checkeditem.cpp \
    Models/checkedlistmodel.cpp \
    Models/listitem.cpp \
    Models/abstractlistmodel.cpp \
    Models/sqllistmodel.cpp \
    Config/configfile.cpp \
    Models/checkedsqllistmodel.cpp \
    Models/filteringcolumnitem.cpp \
    Models/basesqllistmodel.cpp \
    Models/tablemodel.cpp \
    Worker/myrunnable.cpp \
    Models/pivotmodel.cpp \
    Worker/databaseworker.cpp


HEADERS +=  \
            Models/listmodel.h \
            Models/program.h \
            application.h \
            UIController/controller.h \
            UIController/uiservices.h \
            Worker/worker.h \
    IO/csvdatatable.h \
    Models/checkeditem.h \
    Models/checkedlistmodel.h \
    Models/listitem.h \
    Models/abstractlistmodel.h \
    Models/sqllistmodel.h \
    Config/configfile.h \
    Models/checkedsqllistmodel.h \
    Models/filteringcolumnitem.h \
    Models/basesqllistmodel.h \
    Models/tablemodel.h \
    mysqldatabase.h \
    Worker/myrunnable.h \
    Models/pivotmodel.h \
    Worker/databaseworker.h


installPath = $$(MYLIBRARY)/$$QT_VERSION
target.path = $$installPath
INSTALLS += target

installIncludePath = $$installPath/include/QmlApplication

# add support for EXCEL
win32 {
    QT += axcontainer

    SOURCES += Excel/excel.cpp \
               Excel/excelrange.cpp \
               Excel/excelsheet.cpp

    HEADERS += Excel/excel.h \
               Excel/excelrange.h \
               Excel/excelsheet.h

   excel.files = Excel/*.h
   excel.path = $$installIncludePath/Excel
   INSTALLS += excel
}

app.files = *.h
app.path = $$installIncludePath
INSTALLS += app

config.files = Config/*.h
config.path = $$installIncludePath/Config
INSTALLS += config

io.files = IO/*.h
io.path = $$installIncludePath/IO
INSTALLS += io

models.files = Models/*.h
models.path = $$installIncludePath/Models
INSTALLS += models

uicontroller.files = UIController/*.h
uicontroller.path = $$installIncludePath/UIController
INSTALLS += uicontroller

worker.files = Worker/*.h
worker.path = $$installIncludePath/Worker
INSTALLS += worker
