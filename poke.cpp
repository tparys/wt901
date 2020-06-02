#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include "serial.h"
#include "bindump.h"

// Read minimum N bytes
int readn(int fd, void *buf, int count);

int main(int argc, char **argv)
{
    // Check args
    if (argc < 5)
    {
        printf("Usage:\n"
               "  poke <dev> <baud> <reg> <value>\n");
        return -1;
    }

    // Open device
    int fd = serial_open(argv[1], atoi(argv[2]));

    // Register
    unsigned regnum = strtoul(argv[3], NULL, 0);
    unsigned value = strtoul(argv[4], NULL, 0);

    // Set up command
    char cmd[5];
    cmd[0] = 0xff;
    cmd[1] = 0xaa;
    cmd[2] = regnum;
    cmd[3] = value & 0xff;
    cmd[4] = (value >> 8) & 0xff;

    // Outbound command
    bindump("TX CMD", cmd, 5);

    // Write command
    if (write(fd, cmd, 5) != 5)
    {
        perror("Cannot write to device!");
    }

    // Cleanup
    close(fd);
    return 0;
}

// Read minimum N bytes
int readn(int fd, void *buf, int count)
{
    char *ptr = (char*)buf;
    int left = count;

    do
    {
        // Attempt next read
        int rc = read(fd, ptr, left);
        if (rc < 1)
        {
            return -1; 
        }

        // Update counters
        ptr += rc;
        left -= rc;

    } while (left > 0);

    return count;
}

