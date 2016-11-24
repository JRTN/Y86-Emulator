#include <string.h>
#include <stdio.h>
#include "util.h"

/*
    Searches an array of strings for the given target and
    returns the position of the first encounter 
*/
int searchStringArray(char **arr, char *target) {
    int x = 0;
    while(arr[x]) {
        if(strcmp(arr[x], target) == 0) {
            return x;
        }
        x++;
    }
    return -1;
}

int32_t hexToDec(char *hex) {
    int32_t result;
    sscanf(hex, "%x", &result);
    return result;
}
