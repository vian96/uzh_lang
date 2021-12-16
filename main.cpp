#include "lexer/lexer.h"

#include <stdio.h>

int main ()
    {
    LexemFile code = read_code_lexem ("code.uzh");
    for (int i = 0; i < code.cnt; i++)
        {
        Lexem *temp = code.lexs + i;
        switch (temp->type)
        {
        case LEX_ID:
        case LEX_SYM:
            printf ("%s ", temp->str);
            break;

        case LEX_NUM:
            printf ("%d ", temp->number);
            break;

        case LEX_SPACE:
            printf ("\nLINE%*s", temp->cnt, "");
            break;

        default:
            printf ("ERROR: unknown type of lex\n");
            break;
        }
        }

    
    }
