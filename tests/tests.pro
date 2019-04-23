QT += testlib
QT += gui widgets sql
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES += \ 
    main.cpp \
    tst_sqlquerymodel.cpp \
    tst_markupdocument.cpp

HEADERS += \
    tst_sqlquerymodel.h \
    tst_markupdocument.h

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/analyzer
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(analyzer)

INCLUDEPATH += $$(MYLIBRARY)/$$QT_VERSION/include/QmlApplication
LIBS += -L$$(MYLIBRARY)/$$QT_VERSION -l$$qtLibraryTarget(QmlApplication)

DISTFILES += \
    doc1.html

RESOURCES += \
    data.qrc
