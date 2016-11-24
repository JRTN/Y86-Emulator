#include <string.h>
#include <stdio.h>
#include "util.h"

/*
    Searches an array of strings for the given target and
    returns the position of the first encounter.
    Arguments:
        char **arr - the array of strings to be searched
        char *target - the string to search for
    Return:
        The position at which target is first encountered if
        target is present in the array; -1 otherwise
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

/*
    Converts a hexadecimal string representatiion of a 32 bit number
    into a 32 bit integer. 
    Arguments:
        char *hex - the hexadecimal string representation of the number
    Return:
        An int32_t containing the value of the hex string if no errors
        are encountered; -1 otherwise.
*/
int32_t hexToDec(char *hex) {
    int32_t result;
    int res = sscanf(hex, "%x", &result);
    if(!res) {
        return -1;
    }
    return result;
}
