#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include "util.h"

/*
    Simple linear search that searches an array of strings for the 
    given target and returns the position of the first encounter.
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
    sscanf(hex, "%x", &result);
    return result;
}

int32_t hexToDecLittleEndian(char *hex) {
    int32_t result = 0;
    result = hexToDec(hex);
    result = ( (result & 0xff) << 24 ) | ( (result & 0xff00) << 8 ) | ( (result & 0xff0000) >> 8 ) | ( (result >> 24) & 0xff ); /* Trust me */
    return result;
}

/*
    Copies the first n characters from a c string and creates a new null-terminated c string
    out of the copied characters.
    Arguments:
        const char *src - the string from which the characters will be copied
        size_t n - the number of characters to be copied
    Return:
        A new null terminated c string containing the copied characters if successfull; null otherwise
*/
char *nt_strncpy(const char *src, size_t n) {
    char *result = calloc(n + 1, sizeof(char));
    if(result) {
        result[n] = '\0';
        strncpy(result, src, n);
    }
    return result;
}
