#ifndef util_h
#define util_h
#include <stdint.h>
#include <ctype.h>

#define STREQ(x,y) strcmp(x,y)==0

int searchStringArray(char**, char*);
int32_t hexToDec(char*);
int32_t hexToDecLittleEndian(char*);
char *nt_strncpy(const char*, size_t);
void printInt32LittleEndian(int32_t);

#endif
