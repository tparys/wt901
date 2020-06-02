#include <unistd.h>
#include <termios.h>
#include <stdexcept>
#include "serial.h"
#include "bindump.h"
#include "WT901.h"

#define GET_LE16(ptr, off) ((int16_t)(ptr[off] | (ptr[off+1] << 8)))
#define GET_LEFLOAT(ptr, off) (GET_LE16(ptr, off) / 32768.)

/// Constructor
WT901::WT901(char const *path, int baud)
{
    // Open serial device
    fd_ = serial_open(path, baud);

    // Suspend output
    setReg(0x03, 0x0c);

    // Drain input
    usleep(10000);
    tcflush(fd_, TCIOFLUSH);

    // Select time, accel, gyro, & mag
    setReg(0x02, 0x17);

    // Minimum read size to read all data
    setMinRead(4 * 11);

    // Restart at 10 Hz data
    setReg(0x03, 0x06);
}

/// Set register
void WT901::setReg(uint8_t reg_num, uint16_t reg_value)
{
    // Create command
    char cmd[5];
    cmd[0] = 0xff;
    cmd[1] = 0xaa;
    cmd[2] = reg_num;
    cmd[3] = reg_value & 0xff;
    cmd[4] = (reg_value >> 8) & 0xff;

    bindump("TX", cmd, 5);

    // Send command
    if (write(fd_, cmd, 5) != 5)
    {
        throw std::runtime_error("Short write on serial port");
    }

    usleep(100000);
}

// Get new data
bool WT901::getData(ImuData &data)
{
    uint8_t buf[44] = {0};

    // Read data
    if (read(fd_, buf, sizeof(buf)) != 44)
    {
        return false;
        exit(1);
    }

    //bindump("RX", buf, sizeof(buf));

    // Process return
    return getDataTime(data.time, buf + 0) &&
        getDataSensor(data.accel, buf + 11, 0x51, -16) &&
        getDataSensor(data.gyro, buf + 22, 0x52, 2000) &&
        getDataSensor(data.mag, buf + 33, 0x54, 32768);
}

/// Get time data
bool WT901::getDataTime(TimeData &data, uint8_t *ptr)
{
    // Sanity check
    if (!validateSegment(ptr, 0x50))
    {
        return false;
    }

    // Pull data fields
    data.year = ptr[2];
    data.month = ptr[3];
    data.day = ptr[4];
    data.hour = ptr[5];
    data.minute = ptr[6];
    data.sec = ptr[7];
    data.msec = GET_LE16(ptr, 8);

    // All done
    return true;
}

/// Get 3 axis sensor data
bool WT901::getDataSensor(SensorData &data, uint8_t *ptr,
                          uint8_t msg_id, float scale)
{
    // Sanity check
    if (!validateSegment(ptr, msg_id))
    {
        return false;
    }

    // Sensor data
    data.data[0] = GET_LEFLOAT(ptr, 2) * scale;
    data.data[1] = GET_LEFLOAT(ptr, 4) * scale;
    data.data[2] = GET_LEFLOAT(ptr, 6) * scale;

    // Sensor temperature
    data.temp = GET_LE16(ptr, 8) / 100.;

    // All done
    return true;
}

// Validate 11 byte segment
bool WT901::validateSegment(uint8_t *ptr, uint8_t id)
{
    // Basic structure
    if ((ptr[0] != 0x55) &&
        (ptr[1] != id))
    {
        return false;
    }

    // Checksum
    uint8_t cs = 0;
    for (int i = 0; i < 10; i++)
    {
        cs += ptr[i];
    }
    if (ptr[10] != cs)
    {
        return false;
    }

    // Looks OK
    return true;
}

/// Set serial min read size
void WT901::setMinRead(unsigned count)
{
    // Get serial parameters
    struct termios attr;
    if (tcgetattr(fd_, &attr))
    {
        perror("FATAL - Cannot get serial parameters");
        exit(1);
    }

    // Set minimum read size
    attr.c_cc[VMIN] = count;

    // Set serial parameters
    if (tcsetattr(fd_, TCSANOW, &attr))
    {
        perror("FATAL - Cannot set serial parameters");
        exit(1);
    }
}
