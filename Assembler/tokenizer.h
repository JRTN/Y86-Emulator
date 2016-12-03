#ifndef tokenizer_h
#define tokenizer_h

struct TokenizerT_ {
    char *current;
};
typedef struct TokenizerT_ TokenizerT;

char *TKGetNextToken(TokenizerT*);
TokenizerT *TKCreate(char*);
void TKDestroy(TokenizerT*);

#endif
