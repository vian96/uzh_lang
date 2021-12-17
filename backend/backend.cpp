#include "backend.h"

#include <assert.h>

void compile_lang_tree (LangTree *tree, FILE *f_out)
    {
    assert (tree);
    assert (f_out);

    fprintf (f_out, "; This file was autmatically compiled by"
             "Uzh lang compiler\n\njmp :main\n");
    
    }

void compile_node_lang (LangTree *tree, FILE *f_out);
