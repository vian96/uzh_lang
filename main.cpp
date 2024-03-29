// #define CASM // uncomment if you want casm mode

#include "frontend/frontend.h"

#ifndef CASM
    #include "x86backend/backend.h"
#else
    #include "backend/backend.h"
#endif

#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
    {
    if (argc != 2) 
        {
        printf (
            "ERROR: wrong number of arguments\n"
            "You shold call \"uzh *name*.casm\"!\n"
        );
        return 0;
        }

    char *in_name = (char*) calloc (strlen (argv[1]) + 10, sizeof (char));
    strcpy (in_name, argv[1]);
    strcat (in_name, ".uzh");

    Lexem *code = read_code_lexem (in_name);
    free (in_name);
    in_name = nullptr;

    print_lexems (code);
    printf ("\nCompiling\n");
    
    LangTree *tree = lang_frontend (code);
    lang_tree_dump (tree, stdout);
    printf ("\n");
    
    printf ("\nDoing dumps\n");
    lang_tree_graph_dump (tree);

    char *out_name = (char*) calloc (strlen (argv[1]) + 10, sizeof (char));
    strcpy (out_name, argv[1]);

#ifndef CASM
    strcat (out_name, ".asm");
#else
    strcat (out_name, ".casm");
#endif

    FILE *f_out = fopen (out_name, "w");
    free (out_name);
    out_name = nullptr;

    compile_lang (tree, f_out);
    fclose (f_out);
    f_out = nullptr;

    free_lexems (code);
    free_tree_lang (tree);

    // setting asm command
    char *asm_call = (char*) calloc (strlen (argv[1]) + 20, sizeof (char));
#ifndef CASM
    strcpy (asm_call, "x86asm ");
#else
    strcpy (asm_call, "asm ");
#endif
    strcat (asm_call, argv[1]);
    system (asm_call);

    strcpy (asm_call, argv[1]);
#ifndef CASM
    strcat (out_name, ".asm");
#else
    strcat (out_name, ".casm");
#endif
    printf ("Removing %s\n", asm_call);
    // remove (asm_call);

    free (asm_call);
    asm_call = nullptr;

    printf ("DONE\n");
    }
