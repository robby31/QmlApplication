QT += testlib
QT += gui widgets sql

CONFIG += qt warn_on depend_includepath testcase
CONFIG -= app_bundle
CONFIG += sdk_no_version_check

TEMPLATE = app

SOURCES += \ 
    main.cpp \
    tst_sqlquerymodel.cpp \
    tst_markupdocument.cpp

HEADERS += \
    tst_sqlquerymodel.h \
    tst_markupdocument.h

INCLUDEPATH += $$(MYLIBRARY)/include/analyzer
LIBS += -L$$(MYLIBRARY)/lib -l$$qtLibraryTarget(analyzer)

INCLUDEPATH += $$(MYLIBRARY)/include/QmlApplication
LIBS += -L$$(MYLIBRARY)/lib -l$$qtLibraryTarget(QmlApplication)

DISTFILES += \
    doc1.html \
    doc1.xml \
    doc2.html \
    doc3.html \
    doc4.html

RESOURCES += \
    data.qrc
