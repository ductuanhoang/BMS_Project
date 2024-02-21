#ifndef PARSERMESSAGE_H
#define PARSERMESSAGE_H
#include <QString>


#define MAX_CELLS_SUPPORT (15)
#define MAX_PACKS_SUPPORT (15)
#define MAX_TEMPERATURE_SENSORS (3)



typedef struct {
    uint8_t error;
}error_t;

typedef struct
{
    uint8_t pack_id;
    uint8_t status;
    uint16_t cell_voltage[MAX_CELLS_SUPPORT];
    uint16_t total_voltage;
    int current;
    int temperature[MAX_TEMPERATURE_SENSORS];
    uint16_t soc;
    uint8_t error;
}battery_struct_t;

bool parserMessage(const QString &message, battery_struct_t *data);

#endif // PARSERMESSAGE_H
