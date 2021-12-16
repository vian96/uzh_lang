#include "lexer/lexer.h"

#include <stdio.h>

int main ()
    {
    LexemFile code = read_code_lexem ("code.uzh");
    print_lexems (code);
    free_lexems (code);
    
    }
