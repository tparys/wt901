#ifndef SERIAL_H
#define SERIAL_H

/**
 * Open Serial Device
 *
 * \param[in] path Path to device
 * \param[in] baud Desired bitrate
 * \return Descriptor or -1 on error
 */
int serial_open(char const *path, int baud);

#endif
