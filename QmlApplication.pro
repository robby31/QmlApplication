#-------------------------------------------------
#
# Project created by QtCreator 2014-10-22T13:15:24
#
#-------------------------------------------------

TEMPLATE = lib

TARGET = $$qtLibraryTarget(QmlApplication)

CONFIG += debug_and_release
CONFIG += shared_and_static build_all

QT       += qml quick widgets sql charts
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

CONFIG += c++14

SOURCES +=  \
    Document/markupdocmodel.cpp \
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
    Worker/databaseworker.cpp \
    SqlModel/myvxymodelmapper.cpp \
    SqlModel/sqlquerymodel.cpp \
    SqlModel/sqltablemodel.cpp \
    Document/markupdocument.cpp \
    Document/markupblock.cpp \
    debuginfo.cpp \
    debuginfoitem.cpp \
    debuginfomodel.cpp


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
    debuginfo.h \
    debuginfoitem.h \
    debuginfomodel.h \
    mysqldatabase.h \
    Worker/myrunnable.h \
    Models/pivotmodel.h \
    Worker/databaseworker.h \
    SqlModel/myvxymodelmapper.h \
    SqlModel/sqlquerymodel.h \
    SqlModel/sqltablemodel.h \
    mynetwork.h \
    Document/markupdocument.h \
    Document/markupdocmodel.h \
    Document/markupblock.h

#DEFINES += PROFILING
INCLUDEPATH += $$(MYLIBRARY)/include/analyzer
LIBS += -L$$(MYLIBRARY)/lib -l$$qtLibraryTarget(analyzer)

installPath = $$(MYLIBRARY)
target.path = $$installPath/lib
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

document.files = Document/*.h
document.path = $$installIncludePath/Document
INSTALLS += document

io.files = IO/*.h
io.path = $$installIncludePath/IO
INSTALLS += io

models.files = Models/*.h
models.path = $$installIncludePath/Models
INSTALLS += models

sql_models.files = SqlModel/*.h
sql_models.path = $$installIncludePath/SqlModel
INSTALLS += sql_models

uicontroller.files = UIController/*.h
uicontroller.path = $$installIncludePath/UIController
INSTALLS += uicontroller

worker.files = Worker/*.h
worker.path = $$installIncludePath/Worker
INSTALLS += worker

macx {
    CONFIG += lib_bundle

    FRAMEWORK_HEADERS.version = Versions
    FRAMEWORK_HEADERS.files = $${HEADERS}
    FRAMEWORK_HEADERS.path = include
    QMAKE_BUNDLE_DATA += FRAMEWORK_HEADERS

    QMAKE_FRAMEWORK_BUNDLE_NAME = QmlApplication
}
