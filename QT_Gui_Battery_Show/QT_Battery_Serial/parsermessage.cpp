#include "parsermessage.h"
#include <QtGlobal>
#include <QtLogging>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#define CURRENT_OFFSET  (300*100) //300A
#define TEMPERATURE_OFFSET  (273*100) // 273 degree
void checkConnection(void)
{
 // create thread check connection
}

bool parserMessage(const QString &message, battery_struct_t *data)
{
    bool ret = false;
    QString data_test = "{\"battery_id\":1,\"cells\":[1512,1603,17], \"temperature\":[123,456], \"soc\":100, \"err\":8, \"status\":8}";

    QByteArray jsonData = message.toUtf8();
    if(jsonData.isEmpty() == true)
    {
        qDebug() << "Need to fill JSON data";
        return false;
    }

    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonData);
    if(jsonDocument.isObject() == false) {
        qDebug() << "It is not a JSON object";
        return false;
    }

    QJsonObject object = jsonDocument.object();

    QJsonValue id = object.value("battery_id");
    QJsonArray cells = object.value("cells").toArray();
    QJsonArray temperature = object.value("temperature").toArray();
    QJsonValue soc = object.value("soc");
    QJsonValue err = object.value("err");
    QJsonValue current = object.value("current");
    QJsonValue status = object.value("status");
    // show ID of battery
    qDebug() << "Battery ID = " + QString::number(id.toInteger());
    // show number of cells
    qDebug() << "Number of cells = " + QString::number(cells.size());
    // show number of temperatures
    qDebug() << "Number of temperature = " + QString::number(temperature.size());
    data->pack_id = id.toInteger();
    data->error = err.toInteger();
    data->soc = soc.toInteger();
    data->status = status.toInteger();
    if(data->status == 1)
        data->current = current.toInteger() - CURRENT_OFFSET;
    else
        data->current = current.toInteger();
    // add cells voltage value
    data->total_voltage = 0;
    for (int var = 0; var < cells.size(); ++var) {
        data->cell_voltage[var] = cells.at(var).toInt();
        qDebug() << "cell at " + QString::number(var) + " is " + QString::number(data->cell_voltage[var]);
        data->total_voltage += data->cell_voltage[var];
    }
    // add temperatures value
    for (int var = 0; var < temperature.size(); ++var) {
        if(data->status == 1)
            data->temperature[var] = temperature.at(var).toInt() - TEMPERATURE_OFFSET;
        else
            data->temperature[var] = temperature.at(var).toInt();

        qDebug() << "temperature at " + QString::number(var) + " is " + QString::number(data->temperature[var]);
    }
    ret = true;
    return ret;
}
