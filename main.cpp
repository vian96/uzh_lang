#include "lexer/lexer.h"
#include "frontend/frontend.h"

#include <stdio.h>

int main ()
    {
    Lexem *code = read_code_lexem ("code.uzh");
    print_lexems (code);
    printf ("\n");
    
    LangTree *tree = lang_frontend (code);
    lang_tree_dump (tree, stdout);
    printf ("\n");

    free_lexems (code);
    free_tree_lang (tree);

    printf ("DONE\n");
    }
