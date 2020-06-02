#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
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
    double dpr = 180 / M_PI;
    while (imu.getData(data))
    {
        // Local copies to make code more readable
        double ax = data.accel.data[0];
        double ay = data.accel.data[1];
        double az = data.accel.data[2];

        // Cheap and very noisy attempt at inclination
        printf("\nUpdate: %u\n", data.time.msec);
        printf("  - acc.x = %f\n", data.accel.data[0]);
        printf("       .y = %f\n", data.accel.data[1]);
        printf("       .z = %f\n", data.accel.data[2]);
        printf("  - pitch = %f\n", dpr * atan2(-ax, az));
        printf("  - roll  = %f\n", dpr * atan2(ay, az));
    }

    return 0;
}
