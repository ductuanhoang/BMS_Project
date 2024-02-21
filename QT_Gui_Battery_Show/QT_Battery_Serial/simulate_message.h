#ifndef SIMULATE_MESSAGE_H
#define SIMULATE_MESSAGE_H

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

typedef struct
{
    uint16_t battery_id; //
    float cell_voltage[15];
    float current;
    float temperature[3];
    float soc;
} simulate_message_t;

QString simulate_packet_message(simulate_message_t data);

/**
 * @brief simulate_packet_non_json_message
 * {"data":[13][1.2][80.1][3.2,3.3,3.4][12.3,11.8,13.2,12.0,11.5,12.6,11.9,12.8,13.1,11.7,12.2,12.4]}<CR><LF>
 * @param data
 * @return
 */
QString simulate_packet_non_json_message(simulate_message_t data);

#endif // SIMULATE_MESSAGE_H
