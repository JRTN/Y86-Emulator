/*
 * tokenizer.c
 * Author: John Russell
 * Date Created: September 16, 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tokenizer.h"

/*
 * Called when a call to malloc or realloc fails. Informs the user that the program failed to allocate memory, then
 * exits the program with an error code.
 */
static void failedToAllocateMemory() {
    printf("\nFailed to allocate memory. Program is exiting.");
    exit(EXIT_FAILURE);
}

/*
 * Helper function that handles the building of the token until more white space or the end of the string is reached.
 */
static char *buildToken(TokenizerT *tk) {
    /* Create a new c string that will be returned */
    size_t size = 0;
    char *token = NULL;
    while (*tk->current && !isspace(*tk->current)) {
        size++;
        token = realloc(token, size);
        if (!token) {
            failedToAllocateMemory();
        }
        token[size - 1] = *tk->current;
        tk->current++;
    }
    /* Increase the size of the array by 1 and terminate it with the null character */
    token = realloc(token, size + 1);
    if (!token) {
        failedToAllocateMemory();
    }
    token[size] = '\0';
    return token;
}

static char *buildString(TokenizerT *tk) {
    tk->current++; /* Advance past the first quotation mark */
    size_t size = 0;
    char *token = NULL;
    while (*tk->current && *tk->current != '"') {
        size++;
        token = realloc(token, size);
        if (!token) {
            failedToAllocateMemory();
        }
        token[size - 1] = *tk->current;
        tk->current++;
    }
    /* Increase the size of the array by 1 and terminate it with the null character */
    token = realloc(token, size + 1);
    if (!token) {
        failedToAllocateMemory();
    }
    token[size] = '\0';
    /* Advance past the second quotation mark if we haven't reached the end of the file */
    if(*tk->current) {
        tk->current++;
    }
    return token;
}

/*
 * Gets the next token ahead of the current position of the tokenizer. Dynamically allocates memory for the token, building
 * it one character at a time. If there are tokens remaining, the next token is returned as a NULL-terminated C string.
 * If there are no tokens left, NULL is returned. It is the caller's responsibility to free the memory holding the tokens.
 */
char *TKGetNextToken(TokenizerT *tk) {
    while (*tk->current && isspace(*tk->current) && *tk->current != '"') {
        tk->current++;
    }
    char *result = NULL;
    if(!*tk->current) {
        return NULL;
    } else if(*tk->current == '"') {
        result = buildString(tk);
    } else {
        result = buildToken(tk);
    }
    return result;
}


/*
 * Creates an instance of a TokenizerT object for a given input string. The initial position of the tokenizer is at the
 * first character in the string. It is the caller's responsibility to ensure TKDestroy is called when the TokenizerT
 * object is no longer needed.
 */
TokenizerT *TKCreate(char *ts) {
    TokenizerT *tokenizer = malloc(sizeof(TokenizerT));
    if (!tokenizer) {
        return NULL;
    }
    tokenizer->current = ts;
    return tokenizer;
}

/*
 * Destroys a TokenizerT object by freeing all memory dynamically allocated for the object. This
 * should be called whenever a TokenizerT object is no longer needed.
 */
void TKDestroy(TokenizerT *tk) {
    free(tk);
}
