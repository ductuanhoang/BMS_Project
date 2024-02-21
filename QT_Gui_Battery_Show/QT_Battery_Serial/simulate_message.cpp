#include "simulate_message.h"
#include <QtGlobal>
#include <QtLogging>

/**
 * @brief simulate_packet_message
 *
 * {"battery_id":4,"cells":[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0],"temperature":[0,0,0],"current":0,"soc":0,"err":8,"status":0}
 *
 */
QString simulate_packet_message(simulate_message_t data)
{
    QJsonObject jsonObject;
    jsonObject["battery_id"] = data.battery_id;

    QJsonArray cellVoltageArray;
    for (int i = 0; i < 12; ++i) {
        cellVoltageArray.append(QString::number(data.cell_voltage[i]));
    }
    jsonObject["cell_voltage"] = cellVoltageArray;

    jsonObject["current"] = QString::number(data.current);

    QJsonArray temperatureArray;
    for (int i = 0; i < 3; ++i) {
        temperatureArray.append(QString::number(data.temperature[i]));
    }
    jsonObject["temperature"] = temperatureArray;

    jsonObject["soc"] = QString::number(data.soc);

    QJsonDocument doc(jsonObject);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}

/**
 * @brief simulate_packet_non_json_message
 * {"data":[13][1.2][80.1][3.2,3.3,3.4][12.3,11.8,13.2,12.0,11.5,12.6,11.9,12.8,13.1,11.7,12.2,12.4]}<CR><LF>
 * @param data
 * @return
 */
QString simulate_packet_non_json_message(simulate_message_t data)
{
    char buffer[256];
    sprintf(buffer, "{\"data\":[%d][%0.2f][%0.2f][%0.2f,%0.2f,%0.2f][%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f,%0.2f]}",
    data.battery_id,
    data.current,
    data.soc,
    data.temperature[0],data.temperature[1],data.temperature[2],
    data.cell_voltage[0], data.cell_voltage[1], data.cell_voltage[2],
    data.cell_voltage[3], data.cell_voltage[4], data.cell_voltage[5],
    data.cell_voltage[6], data.cell_voltage[7], data.cell_voltage[8],
    data.cell_voltage[9], data.cell_voltage[10], data.cell_voltage[11],
    data.cell_voltage[12], data.cell_voltage[13], data.cell_voltage[14]
    );
    return QString(buffer);
}
/**
 * @brief simulate_packet_message_cell1
 *
 * {"battery_id":4,"cells":[0,0,0]}
 *
 */
QString simulate_packet_message_cell1(simulate_message_t data)
{
    QJsonObject jsonObject;
    jsonObject["battery_id"] = data.battery_id;

    QJsonArray cellVoltageArray;
    for (int i = 0; i < 3; ++i) {
        cellVoltageArray.append(QString::number(data.cell_voltage[i]));
    }
    jsonObject["cell1"] = cellVoltageArray;

    QJsonDocument doc(jsonObject);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}
/**
 * @brief simulate_packet_message_cell2
 *
 * {"battery_id":4,"cells":[0,0,0]}
 *
 */
QString simulate_packet_message_cell2(simulate_message_t data)
{
    QJsonObject jsonObject;
    jsonObject["battery_id"] = data.battery_id;

    QJsonArray cellVoltageArray;
    for (int i = 0; i < 3; ++i) {
        cellVoltageArray.append(QString::number(data.cell_voltage[i]));
    }
    jsonObject["cell2"] = cellVoltageArray;

    QJsonDocument doc(jsonObject);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}
