#include <QtTest/QtTest>
#include "mrudeque.h"
//#include <QtDebug>

class testDeque : public QObject
{
    Q_OBJECT

    private slots:
        void push();
};

void testDeque::push()
{
    MRUDeque<int> d(5);

    for(int i = 0; i < 13; i++)
    {
        d.push(i);
        QCOMPARE(i, d.at(0));
//        qDebug() << d.toVector();
    }

    MRUDeque<QString> ds(5);
    for(int i = 0; i < 19; i++)
    {
        ds.push("string" + QString::number(i, 10));
        QCOMPARE("string" + QString::number(i, 10), QString(ds.at(0)));
//        qDebug() << ds.toVector();
    }
}
QTEST_MAIN(testDeque)
#include "testdeque.moc"
