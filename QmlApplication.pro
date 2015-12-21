#-------------------------------------------------
#
# Project created by QtCreator 2014-10-22T13:15:24
#
#-------------------------------------------------

QT       += qml quick widgets sql axcontainer

QT       -= gui

TARGET = QmlApplication

TEMPLATE = lib

DESTDIR = C:/Users/TO26027/Documents/MyLibrary/$$QT_VERSION

DEFINES += QMLAPPLICATION_LIBRARY

SOURCES +=  \
            Models/listmodel.cpp \
            Models/program.cpp \
            application.cpp \
            UIController/controller.cpp \
            UIController/uiservices.cpp \
            Worker/worker.cpp \
    Python/pythonmodule.cpp \
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
    Excel/excel.cpp \
    Excel/excelrange.cpp \
    Excel/excelsheet.cpp

HEADERS +=  libqmlapplication_global.h \
            Models/listmodel.h \
            Models/program.h \
            application.h \
            UIController/controller.h \
            UIController/uiservices.h \
            Worker/worker.h \
    Python/pythonmodule.h \
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
    Excel/excel.h \
    Excel/excelrange.h \
    Excel/excelsheet.h


# add Python library
include ( $$PWD/../EPC2/PythonQt/build/common.prf )
include ( $$PWD/../EPC2/PythonQt/build/PythonQt.prf )

DISTFILES += \
    QmlApplication.prf

