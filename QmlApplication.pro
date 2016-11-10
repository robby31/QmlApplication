#-------------------------------------------------
#
# Project created by QtCreator 2014-10-22T13:15:24
#
#-------------------------------------------------

TEMPLATE = lib
TARGET = QmlApplication
QT       += qml quick widgets sql
QT       -= gui

qtHaveModule(webview) {
    QT += webview
    DEFINES += WEBVIEW_PACKAGE
}

TARGET = $$qtLibraryTarget($$TARGET)

!exists($$(MYLIBRARY)) {
    error("variable MYLIBRARY not set.")
}

DESTDIR = $$(MYLIBRARY)/$$QT_VERSION

DEFINES += QMLAPPLICATION_LIBRARY

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
    Worker/myrunnable.cpp


HEADERS +=  libqmlapplication_global.h \
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
    Worker/myrunnable.h


# add support for EXCEL
win32 {
    QT += axcontainer

    SOURCES += Excel/excel.cpp \
               Excel/excelrange.cpp \
               Excel/excelsheet.cpp

    HEADERS += Excel/excel.h \
               Excel/excelrange.h \
               Excel/excelsheet.h
}

DISTFILES += \
    QmlApplication.prf

