/*
   Project    : Lexical Analyzer
   Author     : Vangimalla Naveen Kumar Reddy
   Batch ID   : 26015_092
   Language   : C
   File       : main.h

   DESCRIPTION
   -----------
        A C utility that reads a C source file, tokenizes it, and reports every token's line, column, type, and lexeme — plus a summary and an
   Excel export. Pure C: no Flex, no Lex, no libraries.

   Token types:
        KEYWORD  IDENTIFIER  NUMBER  FLOAT  STRING  CHAR OPERATOR  PUNCTUATION  UNKNOWN  ERROR  EOF

   -----------------------------------------------------------------------------
   USAGE
   -----------------------------------------------------------------------------

       $ ./a.out <source_file> [-v]

       -v      Verbose: adds numeric VALUE column for NUMBER / FLOAT

   Menu:
       1  DO LEXICAL ANALYZE   tokenize + write Output.xls
       2  GET SUMMARY          per-type counts + total errors
       3  GET XL Sheet         confirm Output.xls
       4  EXIT

   -----------------------------------------------------------------------------
   SAMPLE OUTPUT
   -----------------------------------------------------------------------------

   $ ./a.out sample.c -v

       ----------------------------------------------------------------------
       | ROW |COLUMN|TYPE        |LEXICAL                     |VALUE        |
       ----------------------------------------------------------------------
       |1    |2     |KEYWORD     |int                         |             |
       |1    |6     |IDENTIFIER  |main                        |             |
       |2    |18    |NUMBER      |42                          |42           |
       |3    |18    |FLOAT       |3.14e2                      |314          |
       |5    |1     |EOF         |<EOF>                       |             |
       ----------------------------------------------------------------------

   Summary (menu 2):

       KEYWORDS : 5    IDENTIFIERS : 3    INTEGERS : 2
       FLOATS   : 1    STRINGS     : 0    CHARS    : 1
       OPERATORS: 4    PUNCTUATION : 7    UNKNOWN  : 0
       ERRORS   : 0

   Errors (exit code 2):

       |10 |10 |ERROR |Unterminated Char  |
       |11 |10 |ERROR |Unterminated Char  |
       |12 |11 |ERROR |unterminated string|
       ERRORS : 3

   -----------------------------------------------------------------------------
   FILES
   -----------------------------------------------------------------------------

       main.c      - args, menu, output
       token.c     - tokenizer (scan_alpha/number/string/char, comments)
       type.c      - keywords, type names
       creat_xls.c - CSV writer (xl_header, xl_content)
       main.h      - structs, enum, prototypes

   -----------------------------------------------------------------------------
   EXIT CODES
   -----------------------------------------------------------------------------

       0 = clean      2 = errors      1 = bad arguments
*/

#include<stdio.h>
#include"main.h"

int main(int argc,char*argv[])
{
    if(argc<2 || argc >3)
    {
        printf("Invaildi Argument Count.....\n");
        return 0;
    }
    FILE*fp=fopen(argv[1],"r");
    if(fp==NULL)
    {
        printf("Error to open file....\n");
        return 0;
    }
    int verbose=0;
    if (argc >= 3 && strcmp(argv[2], "-v") == 0)
    {
        verbose=1;
    }
    FILE *xfp = fopen("Output.xls", "w");
    if (xfp == NULL) 
    {
        perror("Cannot create Output.xls");
        fclose(fp);
        return 0;
    }
    int count[TOKEN_TYPE_COUNT]={0};
    int error=0;
    Token tok;
    Lexer lex; 
    int op=0,do_analyze=0;
    intitilize(&lex,fp);
    printf("\n\n===============================   LEXICAL ANALYZER  =======================================\n\n");
    do{
        printf("\nMENU :-\n-----------\n");
    printf("1. DO LEXICAL ANALYZE\n2. GET SUMMARY\n3. GET XL Sheet\n4. EXIT\n");
    scanf("%d",&op);
    switch(op){
        case 1:
    rewind(fp);
    intitilize(&lex, fp);
    memset(count, 0, sizeof(count));
    error = 0;

    // restart the sheet 
    rewind(xfp);
    freopen("Output.xls", "w", xfp);               
    xl_header(xfp, verbose);
    if(verbose)
    {
        printf("\n-------------------------------------------------------------------------------------------\n");   
        printf("|%-5s|%-6s|%-15s|%-50s|%-5s|\n"," ROW ","COLUMN","         TYPE","             LEXICAL","  VALUE  ");
        printf("-------------------------------------------------------------------------------------------\n");
    }
    else{
        printf("\n---------------------------------------------------------------------------------\n");
        printf("|%-5s|%-6s|%-15s|%-50s|\n"," ROW ","COLUMN","TYPE","LEXICAL");
        printf("---------------------------------------------------------------------------------\n");
    }
    do
    {
        tok= tokenizer(&lex);
        if(tok.type == TOKEN_ERROR )
        {
            error++;
        }
        if(tok.type < TOKEN_TYPE_COUNT)
        {
            count[tok.type]++;
        }
        if(verbose)
        {
            printf("|%-5d|%-6d|%-15s|%-50s|",tok.line,tok.column,token_type_name(tok.type),tok.lexeme);
            if(tok.type == TOKEN_NUMBER)
            {
                printf("%-9ld|",tok.value.int_val);
            }
            else if(tok.type == TOKEN_FLOAT )
            {
                printf("%-9g|",tok.value.float_val);
            }
            else
                printf("%10s","|");
            printf("\n-------------------------------------------------------------------------------------------\n");
        }
        else
        {
            printf("|%-5d|%-6d|%-15s|%-50s|\n",tok.line,tok.column,token_type_name(tok.type),tok.lexeme);
            printf("---------------------------------------------------------------------------------\n");
        }
        xl_content(xfp,&tok, verbose);
        token_free(&tok);
    }while(tok.type != TOKEN_EOF );
    do_analyze=1;
    break;
    case 2:
    if(do_analyze== 0)
    {
        printf("\nANALYZE NOT DONE YET...\n");
        break;
    }
    printf("\n------------------------------------------------\n");
    printf("Summary:\n-------------\n");
    printf("  KEYWORDS     : %d\n", count[TOKEN_KEYWORD]);
    printf("  IDENTIFIERS  : %d\n", count[TOKEN_IDENTIFIER]);
    printf("  INTEGERS     : %d\n", count[TOKEN_NUMBER]);
    printf("  FLOATS       : %d\n", count[TOKEN_FLOAT]);
    printf("  STRINGS      : %d\n", count[TOKEN_STRING]);
    printf("  CHARS        : %d\n", count[TOKEN_CHAR]);
    printf("  OPERATORS    : %d\n", count[TOKEN_OPERATOR]);
    printf("  PUNCTUATION  : %d\n", count[TOKEN_PUNCTUATION]);
    printf("  UNKNOWN      : %d\n", count[TOKEN_UNKNOWN]);
    printf("  ERRORS       : %d\n\n", error);
    break;
    case 3: 
        printf("XL sheet Created with name ----> ""%s""\n","Output.xls"); break;
    case 4: printf("Conform to Exit (Y/N) : ");
        char choise;
        scanf(" %c",&choise);
        if(choise=='Y' || choise=='y')
        {
            op=-88;
            printf("Program  Terminated Sucessfully\n");
        }
        else{
            printf("EXIT Cancilled Sucessfully....\n");
        }
        break;
    default: printf("Invalid choiss\n");
}
    }while(op != -88);
    fclose(fp);
    fclose(xfp);
    return error ? 2 : 0;
}