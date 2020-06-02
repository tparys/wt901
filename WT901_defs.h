#ifndef WT901_DEFS_H
#define WT901_DEFS_H

/// Time data
struct TimeData
{
    /// Year - 2000
    uint8_t year;

    /// Month
    uint8_t month;

    /// Day
    uint8_t day;

    /// Hour
    uint8_t hour;

    /// Minute
    uint8_t minute;

    /// Second
    uint8_t sec;

    /// Millisecond
    uint16_t msec;
};

/// Triaxial sensor data
struct SensorData
{
    /// XYZ sensor data in specified units
    float data[3];

    /// Sensor temp (C)
    float temp;
};

struct ImuData
{
    /// Time data
    TimeData time;

    /// Accelerometer (g)
    SensorData accel;

    /// Gyroscope (deg/s)
    SensorData gyro;
    
    /// Magnetometer (unit?)
    SensorData mag;
};

#endif
