#ifndef TST_MARKUPDOCUMENT_H
#define TST_MARKUPDOCUMENT_H

#include <QObject>
#include <QtTest>
#include <QDebug>

#include "Document/markupdocument.h"

class tst_markupdocument : public QObject
{
    Q_OBJECT

public:
    tst_markupdocument() = default;

private Q_SLOTS:
    void testCase_Html_Paw_Patrol();
    void testCase_Html4();
    void testCase_Html3();
    void testCase_Html2();
    void testCase_Html();
    void testCase_Xml();
};

#endif // TST_MARKUPDOCUMENT_H
