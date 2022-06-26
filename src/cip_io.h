#ifndef CIP_IO_H
#define CIP_IO_H

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int cip_read_char(char *result, FILE *fp);
int cip_read_short(short *result, FILE *fp);
int cip_read_int(int *result, FILE *fp);

#ifdef __cplusplus
}
#endif

#endif
