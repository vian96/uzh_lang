#include "../tree/lang_tree.h"

#include <stdio.h>

void compile_lang (LangTree *tree, FILE *f_out);

void compile_node_lang (LangTree *tree, FILE *f_out);

// saves global vars at table
void compile_main_branch (LangTree *tree, FILE *f_out);

void compile_tree (LangTree *tree, FILE *f_out);


