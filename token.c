#include "main.h"
#include<stdio.h>

Token tokenizer(Lexer* lx)
{
    char buffer[MAX_LEXEME];
    int len=0;
    int ch;
    do{
        ch=lx_get(lx);
    }while(ch==' ' || ch=='\t' || ch=='\r' || ch=='\n' );

    int line=lx->line;
    int col=lx->column;
    if( ch == EOF)
    {
        return make_token(TOKEN_EOF,"<EOF>",line,col);
    }
    if( isalpha(ch) || ch=='_')
    {
        buffer[len++]=ch;
        return scan_alpha(lx,buffer,&len,line,col);
    }
    if( isdigit(ch))
    {
        buffer[len++]=ch;
        return scan_number(lx,buffer,&len,line,col);
    }
    if(ch=='"')
    {
        return scan_string(lx,buffer,&len,line,col);
    }
    if(ch== '/')
    {
        if(lx_next(lx) == '/' || lx_next(lx)=='*')
        {
            skip_comment(lx);
            return tokenizer(lx);
        }
        buffer[0]='/';
        buffer[1]='\0';
        return make_token(TOKEN_OPERATOR,buffer,line,col);
    }
    if (ch == '\'')
        return scan_char(lx, buffer, &len, line, col);
    if(ch == '=' || ch == '!' || ch == '>' || ch == '<'|| ch == '&' ||
        ch=='|' || ch == '+' || ch == '-' || ch == '*' || ch == '%')
    {
            int n= lx_next(lx);
            int two=0;
            if(n=='=') two=1;
            if(ch=='&' && n=='&') two=1;
            if (ch == '|' && n == '|') two = 1;
            if (ch == '+' && n == '+') two = 1;
            if (ch == '-' && n == '-') two = 1;
            if (ch == '-' && n == '>') two = 1;
            if (ch == '<' && n == '<') two = 1;
            if (ch == '>' && n == '>') two = 1;

            if(two)
            {
                buffer[0]=(char)ch;
                buffer[1]=(char)n;
                buffer[2]='\0';
            }
            else
            {
                buffer[0]=(char)ch;
                buffer[1]='\0';
            }
            return make_token(TOKEN_OPERATOR,buffer,line,col);
    }
    if (ch == '~' || ch == '^') 
    {
        buffer[0] = (char)ch; 
        buffer[1] = '\0';
        return make_token(TOKEN_OPERATOR, buffer, line, col);
    }

    if (strchr("(){}[];,", ch)) 
    {
        buffer[0] = (char)ch; 
        buffer[1] = '\0';
        return make_token(TOKEN_PUNCTUATION, buffer, line, col);
    }

    buffer[0] = (char)ch; 
    buffer[1] = '\0';
    return make_token(TOKEN_UNKNOWN, buffer, line, col);
}

Token make_token(TokenType t,char *s,int line,int col )
{
    Token tok;
    tok.type=t;
    tok.lexeme=strdup(s);
    tok.column=col;
    tok.line=line;
    tok.value.int_val=0;
    return tok;
}

int lx_get(Lexer *lx)
{
    int c= lx->has_next == 1? lx->next : fgetc(lx->src);
    lx->has_next=0;
    if(c=='\n')
    {
        lx->line++;
        lx->column=1;
    }
    else if(c!= EOF)
    {
        lx->column++;
    }
    return c;
}
Token scan_alpha(Lexer*lx,char*buff,int *len,int line,int col)
{
    while(isalnum(lx_next(lx))|| lx_next(lx)=='_')
    {
        if(*len >= MAX_LEXEME-1)
        break;
        buff[(*len)++]=lx_get(lx);
    }
    buff[*len]='\0';
    TokenType t= is_keyword(buff)? TOKEN_KEYWORD :  TOKEN_IDENTIFIER;
    return make_token(t,buff,line,col);
}

int lx_next(Lexer*lx)
{
    if(lx->has_next == 0)
    {
        lx->next=fgetc(lx->src);
        lx->has_next=1;
    }
    return lx->next;
}

Token scan_number(Lexer* lx,char*buf,int *len,int line,int col)
{
    int is_float=0;
    while(isdigit(lx_next(lx)) && *len< MAX_LEXEME-1)
    {
        buf[(*len)++]=lx_get(lx);
    }
    if(lx_next(lx)=='.' && *len< MAX_LEXEME-2)
    {
        is_float=1;
        buf[(*len)++]=lx_get(lx);
        while(isdigit(lx_next(lx)) && *len<MAX_LEXEME-1)
            buf[(*len)++]=lx_get(lx);
    }
    if(lx_next(lx)== 'e' || lx_next(lx) == 'E')
    {
        is_float = 1;
        buf[(*len)++]=lx_get(lx);
        if(lx_next(lx)=='+' || lx_next(lx)=='-')
            buf[(*len)++]= lx_get(lx);
        while(isdigit(lx_next(lx)) && *len< MAX_LEXEME -1)
            buf[(*len)++]= lx_get(lx);
    }
    
    buf[(*len)++]='\0';
    Token tok = make_token(is_float ? TOKEN_FLOAT : TOKEN_NUMBER,buf, line, col);
    if(is_float)
        tok.value.float_val = atof(buf);
    else
        tok.value.int_val=atol(buf);

    return tok;
}

Token scan_string(Lexer* lx,char*buf,int*len,int line,int col)
{
    buf[(*len)++]='"';
    int c;
    while((c =lx_get(lx)) != EOF && c != '"' )
    {
        if(c=='\n')
        {
            buf[*len]='\0';
            return make_token(TOKEN_ERROR, "unterminated string", line, col);
        }
        if(*len >= MAX_LEXEME -3)
            break;
        buf[(*len)++]=(char)c;
        if(c=='\\')
        {
            int n=lx_get(lx);
            if(n == EOF)
                break;
            if(*len < MAX_LEXEME-2)
            buf[(*len)++]= (char)n;
        }
    }
    if(c=='"')
    buf[(*len)++]='"';
    buf[(*len)++]='\0';
    return make_token(TOKEN_STRING,buf,line,col);
}

void skip_comment(Lexer* lx)
{
    if(lx_next(lx)=='/')
    {
        lx_get(lx);
        lx_get(lx);
        while(lx_next(lx) != '\n' && lx_next(lx) !=EOF)
            lx_get(lx);
        return;
    }
    lx_get(lx); 
    lx_get(lx);
    while (lx_next(lx) != EOF) 
    {
        if (lx_next(lx) == '*') 
        {
            if (lx_next(lx) == '/') 
            { 
                lx_get(lx); 
                return; 
            }
        } 
        else {
            lx_get(lx)  ; 
        }
    }
}

Token scan_char(Lexer*lx,char *buf,int *len,int line,int col)
{
    buf[(*len)++]='\'';
    int c=lx_get(lx);
    if(c==EOF || c== '\n')
    {
        return make_token(TOKEN_ERROR,"Unterminated Character",line,col);
    }
    buf[(*len)++]= (char)c;
    if(c=='\\')
    {
        int n=lx_get(lx);
        if(n== EOF)
            return make_token(TOKEN_ERROR,"Bad Escpe",line,col);
        buf[(*len)++]=(char)n;
    }
    if(lx_next(lx) != '\'')
        return make_token(TOKEN_ERROR,"Unterminated Char",line,col);
    
    buf[(*len)++]=(char)lx_get(lx);
    buf[(*len)++]='\0';
    return make_token(TOKEN_CHAR,buf,line,col);
}
void token_free(Token* tok)
{
    if(tok && tok->lexeme)
    {
        free(tok->lexeme);
        tok->lexeme =NULL;
    }
}