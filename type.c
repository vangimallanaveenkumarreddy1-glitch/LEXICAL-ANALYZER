#include "main.h"
#include<stdio.h>


void intitilize(Lexer *lx,FILE*src)
{
    lx->src=src;
    lx->line=1;
    lx->column=1;
    lx->next=0;
    lx->has_next=0;
    return;
}
char *KEYWORDS[] = {
    "auto", "break", "case", "char", "const", "continue","default", "do", "double", "else", "enum", "extern",
    "float", "for", "goto", "if", "inline", "int", "long","register", "restrict", "return", "short", "signed",
    "sizeof", "static", "struct", "switch", "typedef","union", "unsigned", "void", "volatile", "while",
    "_Bool", "_Complx", "_Imaginary",NULL
};

int is_keyword(const char *s) 
{
    for (int i = 0; KEYWORDS[i] != NULL; i++)
        if (strcmp(s, KEYWORDS[i]) == 0)
            return 1;
    return 0;
}
char *token_type_name(TokenType t) 
{
    switch (t) {
        case TOKEN_EOF:         return "EOF";
        case TOKEN_KEYWORD:     return "KEYWORD";
        case TOKEN_IDENTIFIER:  return "IDENTIFIER";
        case TOKEN_NUMBER:      return "NUMBER";
        case TOKEN_FLOAT:       return "FLOAT";
        case TOKEN_STRING:      return "STRING";
        case TOKEN_CHAR:        return "CHAR";
        case TOKEN_OPERATOR:    return "OPERATOR";
        case TOKEN_PUNCTUATION: return "PUNCTUATION";
        case TOKEN_UNKNOWN:     return "UNKNOWN";
        case TOKEN_ERROR:       return "ERROR";
        default:                return "?";
    }
}
