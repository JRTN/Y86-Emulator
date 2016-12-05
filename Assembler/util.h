#ifndef util_h
#define util_h
#include <stdint.h>
#include <ctype.h>

#define LOWERCASESTR(p) for(;*p;++p)*p=tolower(*p);
#define ENDIANSWAP(x) (x>>24)|((x<<8)&0x00ff0000)|((x>>8)&0x0000ff00)|(x<<24)
#define STREQ(x,y) strcmp(x,y)==0

int searchStringArray(char**, char*);
int32_t hexToDec(char*);
int32_t hexToDecLittleEndian(char*);
char *nt_strncpy(const char*, size_t);

#endif
