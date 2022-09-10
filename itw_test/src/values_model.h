#ifndef VALUESMODEL_H
#define VALUESMODEL_H

#include <QObject>

class ValuesModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList values READ getValues NOTIFY sigValuesChanged)
    Q_PROPERTY(QString median READ getMedian NOTIFY sigValuesChanged)
    Q_PROPERTY(bool error READ getError NOTIFY sigValuesChanged)
    Q_PROPERTY(QString message READ getMessage NOTIFY sigValuesChanged)

public:
    ValuesModel();

    Q_INVOKABLE void loadValues(const QString &fileName);

    QStringList getValues() const {return m_values; }
    QString getMedian() const { return m_median; }
    bool getError() const { return m_error; }
    QString getMessage() const { return m_message; }

signals:
    void sigValuesChanged();

private:
    QStringList m_values;
    QString m_median;
    QString m_message;
    bool m_error = false;

    double m_min = -70.0;
    double m_max = 2280000000.0;
};

#endif // VALUESMODEL_H
