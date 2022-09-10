#include <QtTest>

// add necessary includes here
#include "../../src/values_model.h"


class test_median : public QObject
{
    Q_OBJECT

public:
    test_median();
    ~test_median();

private slots:
    void test_case1();

    ValuesModel m_values;
};

test_median::test_median()
{

}

test_median::~test_median()
{

}

void test_median::test_case1()
{
    m_values.loadValues("../../data/sample.json");


    QCOMPARE(m_values.getMedian(), "380432047.17"); // compare two values
}

QTEST_APPLESS_MAIN(test_median)

#include "tst_test_median.moc"
