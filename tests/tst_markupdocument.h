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
    void testCase_Html();

};

#endif // TST_MARKUPDOCUMENT_H
