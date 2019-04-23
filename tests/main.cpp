#include <QApplication>
#include "tst_sqlquerymodel.h"
#include "tst_markupdocument.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setAttribute(Qt::AA_Use96Dpi, true);

    {
        tst_markupdocument test;
        QTest::qExec(&test, argc, argv);
    }

    {
        tst_sqlquerymodel test;
        QTest::qExec(&test, argc, argv);
    }
}
