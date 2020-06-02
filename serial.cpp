#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "serial.h"

// Mapping between integer and bitmask values
struct baud_map
{
    int baud;
    speed_t bitmask;
};

// Helper macro for filling out table
#define MAKE_BAUD(x) { (x), B ## x }

// Known baud rates
static struct baud_map baud_table[] =
{
    MAKE_BAUD(50),
    MAKE_BAUD(75),
    MAKE_BAUD(110),
    MAKE_BAUD(134),
    MAKE_BAUD(150),
    MAKE_BAUD(200),
    MAKE_BAUD(300),
    MAKE_BAUD(600),
    MAKE_BAUD(1200),
    MAKE_BAUD(1800),
    MAKE_BAUD(2400),
    MAKE_BAUD(4800),
    MAKE_BAUD(9600),
    MAKE_BAUD(19200),
    MAKE_BAUD(38400),
    MAKE_BAUD(57600),
    MAKE_BAUD(115200),
    MAKE_BAUD(230400),
    MAKE_BAUD(460800),
    MAKE_BAUD(500000),
    MAKE_BAUD(576000),
    MAKE_BAUD(921600),
    MAKE_BAUD(1000000),
    MAKE_BAUD(1152000),
    MAKE_BAUD(1500000),
    MAKE_BAUD(2000000),
    MAKE_BAUD(2500000),
    MAKE_BAUD(3000000),
    MAKE_BAUD(3500000),
    MAKE_BAUD(4000000),
};

// Lookup bitmask for given baud rate
speed_t serial_get_baud(int baud)
{
    // Number of entries in table
    int num_entries = sizeof(baud_table) / sizeof(baud_map);

    // See if we can find it
    for (int i = 0; i < num_entries; i++)
    {
        if (baud == baud_table[i].baud)
        {
            return baud_table[i].bitmask;
        }
    }

    printf("FATAL - Requested baud rate not found");
    exit(1);
}

/**
 * Open Serial Device
 *
 * \param[in] path Path to device
 * \param[in] baud Desired bitrate
 * \return New file descriptor
 */
int serial_open(char const *path, int baud)
{
    // Open descriptor
    int fd = open(path, O_RDWR | O_NONBLOCK);
    if (fd == -1)
    {
        perror("FATAL - Cannot open requested serial device");
        exit(1);
    }

    // Get serial parameters
    struct termios attr;
    if (tcgetattr(fd, &attr))
    {
        perror("FATAL - Cannot get serial parameters");
        exit(1);
    }

    // Zero all flags
    attr.c_iflag = 0;
    attr.c_oflag = 0;
    attr.c_cflag = 0;
    attr.c_lflag = 0;

    // Configure device 
    cfmakeraw(&attr);
    cfsetspeed(&attr, serial_get_baud(baud));
    
    // Input flags
    attr.c_iflag = IGNBRK;

    attr.c_cflag |= CS8;
    
    attr.c_cflag |= CLOCAL;
    //attr.c_lflag = 

    printf("iflag: %08o\n", attr.c_iflag);
    printf("oflag: %08o\n", attr.c_oflag);
    printf("cflag: %08o\n", attr.c_cflag);
    printf("lflag: %08o\n", attr.c_lflag);

    // Set serial parameters
    if (tcsetattr(fd, TCSANOW, &attr))
    {
        perror("FATAL - Cannot set serial parameters");
        exit(1);
    }

    // Clear nonblock
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);

    // Clear all outstanding I/O
    tcflush(fd, TCIOFLUSH);

    return fd;
}
