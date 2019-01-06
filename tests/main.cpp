#include <QApplication>
#include "tst_sqlquerymodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setAttribute(Qt::AA_Use96Dpi, true);

    {
        tst_sqlquerymodel test;
        QTest::qExec(&test, argc, argv);
    }
}
