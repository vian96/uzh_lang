#include "lexer/lexer.h"
#include "frontend/frontend.h"
#include "backend/backend.h"

#include <stdio.h>

int main ()
    {
    Lexem *code = read_code_lexem ("code.uzh");
    print_lexems (code);
    printf ("\n");
    
    LangTree *tree = lang_frontend (code);
    lang_tree_dump (tree, stdout);
    printf ("\n");
    lang_tree_graph_dump (tree);

    FILE *f_out = fopen ("test.casm", "w");
    compile_lang (tree, f_out);
    fclose (f_out);

    free_lexems (code);
    free_tree_lang (tree);

    printf ("DONE\n");
    }
