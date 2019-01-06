QT += testlib
QT += gui widgets sql
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES += \ 
    main.cpp \
    tst_sqlquerymodel.cpp

HEADERS += \
    tst_sqlquerymodel.h

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/QmlApplication
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(QmlApplication)
