#include <cstdio>
#include "bindump.h"

/**
 * Dump Binary Data to Screen
 *
 * \param[in] name Name/title of binary data
 * \param[in] ptr Pointer to binary data
 * \param[in] len Length of binary data
 */
void bindump(char const *name, void *ptr, int len)
{
    unsigned char *cptr = (unsigned char*)ptr;

    printf("%s:\n", name);
    for (int i = 0; i < len; i++)
    {
        printf(" %02X", cptr[i]);
    }
    printf("\n");
}
