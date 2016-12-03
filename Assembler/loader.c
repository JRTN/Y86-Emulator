#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "loader.h"
#include "tokenizer.h"
#include "util.h"

static char **tokenizeProgram(char*);
static char *getFileContents(char*);

/*
    Calls the appropriate functions to perform the following steps:
        1. Attempt to open and retrive the contents of a file containing the program
        2. Tokenize the contents of the file
        3. Determine the size of the program's memory space and initializes the
           architecture with the appropriate size
        4. Inserts the program data into memory
    Arguments:
        char *fileName - The name of the file containing the program.
    Return:
        1 if the file was successfully opened and its contents loaded into memory;
        0 if there were any issues.
*/
char **getInstructions(char *fileName) {
    char *programString = getFileContents(fileName);
    if(!programString) {
        fprintf(stderr, "ERROR: Failed to open file %s, perhaps it does not exist?\n", fileName);
        return NULL;
    }
    char **programTokens = tokenizeProgram(programString);
    free(programString);
    return programTokens;
}

/*
    Splits a string containing the program at white spaces and stores the tokens
    in an array of c strings. Freeing the memory of the result is left to the
    caller.
    Arguments:
        char *program - The text making up the contents of the program
    Return:
        An array of c strings containing the tokenized program if no issues
        are encountered; NULL otherwise.
*/
static char **tokenizeProgram(char *program) {
    if(!program) {
        return NULL;    
    }
    char **program_tokens = NULL;
    size_t numTokens = 0;
    TokenizerT *tk = TKCreate(program);
    numTokens = 0;

    char *token = NULL;
    while( (token = TKGetNextToken(tk)) ) {
        program_tokens = realloc(program_tokens, sizeof(char*) * ++numTokens);

        if(!program_tokens) {
            fprintf(stderr, "\nMemory allocation failed\n");
            return NULL;
        }

        program_tokens[numTokens - 1] = token;
    }

    program_tokens = realloc(program_tokens, sizeof(char*) * (numTokens + 1));
    program_tokens[numTokens] =  NULL;
    TKDestroy(tk);
    return program_tokens;
}

/*
    Attempts to open a text file and create a string containing its contents.
    Arguments:
        char *fileName - The name of the file
    Return:
        A c string containing the contents of the file if there were no issues;
        NULL otherwise.
*/
static char * getFileContents(char *fileName) {
    char *buffer = NULL;
    long length;
    FILE *f = fopen (fileName, "rb");

    if(f) {
        fseek (f, 0, SEEK_END);
        length = ftell (f);
        fseek (f, 0, SEEK_SET);
        buffer = malloc (length + 1);
        if(!buffer) {
            fprintf(stderr, "Memory allocation failed\n");
            return NULL;
        }

        fread (buffer, 1, length, f);
        buffer[length] = '\0';
        fclose (f);

    }
    return buffer;
}
