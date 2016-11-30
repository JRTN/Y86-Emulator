#ifndef util_h
#define util_h
#include <stdint.h>

int searchStringArray(char**, char*);
int32_t hexToDec(char*);
int32_t hexToDecLittleEndian(char*);
char *nt_strncpy(const char*, size_t);

#endif
