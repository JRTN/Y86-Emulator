#include <stdio.h>
#include "util.h"

union Number_t {
    struct {
        unsigned int one   : 8;
        unsigned int two   : 8;
        unsigned int three : 8;
        unsigned int four  : 8;
    } bytes;
    int32_t num;
};
/*
    Somewhat hacky solution to printing a negative number in little endian hexadecimal
    representation. Works on both positive and negative numbers on little endian machines.
    Arguments:
        int32_t val - the value to be printed in hexadecimal form
*/
void printInt32LittleEndian(int32_t val) {
    union Number_t u;
    u.num = val;
    printf("%02x%02x%02x%02x", u.bytes.one, u.bytes.two, u.bytes.three, u.bytes.four);
}

