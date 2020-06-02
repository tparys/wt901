#ifndef BINDUMP_H
#define BINDUMP_H

/**
 * Dump Binary Data to Screen
 *
 * \param[in] name Name/title of binary data
 * \param[in] ptr Pointer to binary data
 * \param[in] len Length of binary data
 */
void bindump(char const *name, void *ptr, int len);

#endif
