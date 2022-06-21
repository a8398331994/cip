#include <stdio.h>
#include "cip_io.h"

#define READ_BUFFER_SUCCESS 1
#define READ_BUFFER_FAIL 0

/* */
int cip_read_char(char *result, FILE *fp)
{
    int r;
    r = getc(fp);
    if (r == EOF)
        return READ_BUFFER_FAIL;
    if (result)
        *result = r;

    return READ_BUFFER_SUCCESS;
}


/* */
int cip_read_short(short *result, FILE *fp)
{
    int high = 0, low = 0;
    low = getc(fp);
    if (low == EOF) 
        return READ_BUFFER_FAIL;
    high = getc(fp);
    if (high == EOF)
        return READ_BUFFER_FAIL;

    if (result)
        *result = (high << 8) | low;
        
    return READ_BUFFER_SUCCESS;
}

/* */
int cip_read_int(int *result, FILE *fp)
{
    int d[4];
    for (int i = 0; i < 4; i++) {
        if ((d[i] = getc(fp)) == EOF)
            return READ_BUFFER_FAIL;
    }

    int r = 0;
    r |= (d[3] << 24);
    r |= (d[2] << 16);
    r |= (d[1] << 8);
    r |= d[0];

    if (result)
        *result = r;

    return READ_BUFFER_SUCCESS;
}



