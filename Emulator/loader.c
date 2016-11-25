#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "architecture.h"
#include "loader.h"
#include "tokenizer.h"
#include "util.h"

static char **tokenizeProgram(char*);
static char *getFileContents(char*);
static int initializeArchitecture(char**);
static int setInstructions(char**);
static int loadBytes(char**);
static int loadLongs(char**);

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
int loadFileIntoMemory(char *fileName) {
    char *programString = getFileContents(fileName);
    char **programTokens = tokenizeProgram(programString);
    free(programString);
    initializeArchitecture(programTokens);
    setInstructions(programTokens);
    loadBytes(programTokens);
    loadLongs(programTokens);
    return 1;
}

static int loadLongs(char **program) {
    int i = 0;
    while(program[i]) {
        if(strcmp(program[i], LONG_D) == 0) {
            int32_t addr = hexToDec(program[i + 1]);
            int32_t num = atoi(program[i + 2]);
            putLong(num, addr);
        }
        i++;
    }
    printMemory();
    return 1;
}

/*
    Executes the .byte directives in the program
*/
static int loadBytes(char **program) {
    int i = 0;
    while(program[i]) {
        if(strcmp(program[i], BYTE_D) == 0) {
            int32_t addr = hexToDec(program[i + 1]);
            char byte = (char)hexToDec(program[i + 2]);
            putByte(byte, addr);
        }
        i++;
    }
    return 1;
}

/*
    Executes the .text directive in the file, storing the machine instructions
    in memory.
*/
static int setInstructions(char **program) {
    int textPos = searchStringArray(program, TEXT_D);
    if(textPos == -1) {
        return 0;
    }
    int32_t positionInMemory = hexToDec(program[textPos + 1]);
    char *instructions = program[textPos + 2];
    insertInstructions(instructions, positionInMemory);
    return 1;
}

/*
    Finds the position of the size directive within the program tokens and
    initializes the architecture with the given size.
*/
static int initializeArchitecture(char **program) {
    int sizePos = searchStringArray(program, SIZE_D);
    if(sizePos == -1) {
        return 0;
    }
    int32_t programSize = hexToDec(program[sizePos + 1]);
    int success = initialize(programSize);
    return success;
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
