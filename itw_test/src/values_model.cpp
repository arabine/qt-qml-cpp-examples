#include "values_model.h"

#include <QJsonDocument>
#include <QJsonArray>
#include <QFile>
#include <QUrl>

ValuesModel::ValuesModel()
{

}

void ValuesModel::loadValues(const QString &fileName)
{
    QFile loadFile(QUrl(fileName).toLocalFile());

    if (!loadFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        m_message = "Couldn't open file.";
    }

    QByteArray fileData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(fileData));

    double total = 0.0;
    int nbValues = 0;
    m_error = false;
    m_values.clear();

    if (loadDoc.isArray())
    {
        QJsonArray arr = loadDoc.array();
        for (int i = 0; i < arr.size(); i++)
        {
            if (arr[i].isDouble())
            {
                double v = arr[i].toDouble();
                if ((v >= m_min) && (v <= m_max))
                {
                    total += v;
                    nbValues++;
                    m_values.push_back(QString::number(v));
                }
                else
                {
                    m_error = true;
                }
            }
        }
    }

    double mean = total / nbValues;

    m_median = QString::number(mean, 'f', 2);

    emit sigValuesChanged();
}
