#include "main.h"

void xl_header(FILE*fp,int verbose)
{
    if(fp==NULL)
    {
        printf("ERROR OPEN XL SHEET FILE.....\n");
        return ;
    }
    if (verbose)
    fprintf(fp, "\"ROW\",\"COLUMN\",\"TYPE\",\"LEXICAL\",\"VALUE\"\n");
    else
        fprintf(fp, "\"ROW\",\"COLUMN\",\"TYPE\",\"LEXICAL\"\n");
        return;
}

void xl_content(FILE*fp,Token* tok,int verbose)
{
   
    //quote the lexeme so commas/quotes inside it don't break the row 

    fprintf(fp, "%d,%d,\"%s\",\"", tok->line, tok->column,token_type_name(tok->type));

    for (const char *p = tok->lexeme; *p; p++) 
    {
        if (*p == '"') 
        fputc('"', fp);    
        fputc(*p, fp);
    }
    fputc('"', fp);

    if (verbose) 
    {
        if (tok->type == TOKEN_NUMBER)
            fprintf(fp, ",\"%ld\"", tok->value.int_val);
        else if (tok->type == TOKEN_FLOAT)
            fprintf(fp, ",\"%g\"",  tok->value.float_val);
        else
            fputs(",\"\"", fp);
    }
    fputc('\n', fp);
}
