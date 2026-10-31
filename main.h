
#ifndef head
#define head

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_LEXEME 1024
typedef enum {
    TOKEN_EOF = 0,
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_FLOAT,
    TOKEN_STRING,
    TOKEN_CHAR,
    TOKEN_OPERATOR,
    TOKEN_PUNCTUATION,
    TOKEN_UNKNOWN,
    TOKEN_ERROR,
    TOKEN_TYPE_COUNT
} TokenType;

typedef struct {
    TokenType type;
    char*lexeme;
    int line;
    int column;
    union 
    {
        long   int_val;
        double float_val;
    } value;
} Token;

typedef struct {
    FILE *src;
    int   line;
    int   column;
    int   next;
    int   has_next;
} Lexer;

// Function Prototypes
void intitilize(Lexer* lx,FILE*src);
Token tokenizer( Lexer *lex);
char *token_type_name(TokenType t);
int lx_next(Lexer*lx);
Token scan_alpha(Lexer*lx,char*buff,int *len,int line,int col);
int lx_get(Lexer *lx);
Token make_token(TokenType t,char *s,int line,int col );
int is_keyword(const char *s) ;
Token scan_number(Lexer* lx,char*buf,int *len,int line,int col);
Token scan_string(Lexer* lx,char*buf,int*len,int line,int col);
void skip_comment(Lexer* lx);
Token scan_char(Lexer*lx,char *buf,int *len,int line,int col);
void token_free(Token*);
void xl_content(FILE*fp,Token* tok,int verbose);
void xl_header(FILE*fp,int verbose);

#endif