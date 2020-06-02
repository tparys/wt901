#ifndef WT901_H
#define WT901_H

#include <stdint.h>
#include "WT901_defs.h"

class WT901
{
public:

    /// Constructor
    WT901(char const *path, int baud);

    /// Get new data
    bool getData(ImuData &data);

private:

    /// Get time data
    bool getDataTime(TimeData &data, uint8_t *ptr);

    /// Get 3 axis sensor data
    bool getDataSensor(SensorData &data, uint8_t *ptr,
                       uint8_t msg_id, float scale);

    // Validate 11 byte segment
    bool validateSegment(uint8_t *ptr, uint8_t id);

    /// Set register
    void setReg(uint8_t reg_num, uint16_t reg_value);

    /// Set serial min read size
    void setMinRead(unsigned count);

    /// Serial port descriptor
    int fd_;
};

#endif
