#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include "WT901.h"

// Read minimum N bytes
int readn(int fd, void *buf, int count);

int main(int argc, char **argv)
{
    // Check args
    if (argc < 3)
    {
        printf("Usage:\n"
               "  readout <dev> <baud>\n");
        return -1;
    }

    WT901 imu(argv[1], atoi(argv[2]));

    // Main loop
    ImuData data;
    while (imu.getData(data))
    {
        printf("\nUpdate: %u\n", data.time.msec);
        printf("  - acc.x = %f\n", data.accel.data[0]);
        printf("       .y = %f\n", data.accel.data[1]);
        printf("       .z = %f\n", data.accel.data[2]);
        printf("       .t = %f\n", data.accel.temp);
        printf("  - gyr.x = %f\n", data.gyro.data[0]);
        printf("       .y = %f\n", data.gyro.data[1]);
        printf("       .z = %f\n", data.gyro.data[2]);
        printf("       .t = %f\n", data.gyro.temp);
        printf("  - mag.x = %f\n", data.mag.data[0]);
        printf("       .y = %f\n", data.mag.data[1]);
        printf("       .z = %f\n", data.mag.data[2]);
        printf("       .t = %f\n", data.mag.temp);
    }

    return 0;
}
