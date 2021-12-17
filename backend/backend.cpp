#include "backend.h"
#include "name_table/name_table.h"

#include <assert.h>
#include <stdlib.h>

#define is_type(node_, type_) ( (node_)->type == (type_) )
#define is_util(node_, util_) ( ((node_)->type == LT_UTIL) && ((node_)->util == (util_)) )
#define is_oper(node_, oper_) ( ((node_)->type == LT_OPER) && ((node_)->oper == (oper_)) )

#define DEB(...) fprintf (stderr, __VA_ARGS__)

void compile_lang (LangTree *tree, FILE *f_out)
    {
    assert (tree);
    assert (f_out);

    fprintf (f_out, "; This file was autmatically compiled by "
             "Uzh lang compiler\n"
             "; register cx is used for temporary using of var address\n"
             "\njmp :main\n\n");
    
    init_name_tables ();
    compile_main_branch (tree, f_out);
    DEB ("Ended compiling main branch\n");
    
    compile_tree (tree, f_out);
    DEB ("Ended compiling all program\n");
    }

void compile_tree (LangTree *tree, FILE *f_out)
    {
    DEB ("Tree type is %c, util is %c\n", tree->type, tree->util);
    if (is_type (tree, LT_NUM))
        {
        DEB ("Compiling num\n");
        fprintf (f_out, "push %d\n", tree->num);
        return;
        }

    if (is_type (tree, LT_ID))
        {
        DEB ("Compiling var\n");
        bool is_global = 0;
        int size = 0;
        if (tree->right && is_type (tree->right, LT_NUM))
                size = tree->right->num;

        DEB ("Getting index\n");
        int index = get_from_name_table (tree->str, size + 1, &is_global);
        DEB ("Got index\n");
        
        fprintf (f_out, "; getting var %s\n", tree->str);
        if (!is_global)
            fprintf (f_out, "    push bx\n    push %d\n    add\n", index);
        else
            fprintf (f_out, "    push %d\n", index);
        
        if (tree->right)
            {
            fprintf (f_out, "    ; index\n");
            compile_tree (tree->right, f_out);
            fprintf (f_out, "    add\n");
            }        
        fprintf (f_out, "    pop cx\n");

        if (is_util (tree->parent, LTU_ASSIGN))
            fprintf (f_out, "pop [cx]\n");
        else
            fprintf (f_out, "push [cx]\n");
        
        return;
        }

    if (is_util (tree, LTU_FUNC))
        {
        DEB ("Compiling func\n");
        fprintf (f_out, "\n:%s\n", tree->left->str);
        clear_local_names ();
        if (tree->right) 
            compile_tree (tree->right, f_out);
        DEB ("Created local names from param\n");
        return;
        }

    if (is_util (tree, LTU_DEF))
        {
        DEB ("Compiling def\n");
        compile_tree (tree->left, f_out);
        compile_tree (tree->right, f_out);
        return;
        }

    if (is_util (tree, LTU_ASSIGN))
        {
        DEB ("Compiling assing\n");
        compile_tree (tree->right, f_out);
        compile_tree (tree->left, f_out);
        return;
        }

    // TODO reduce copy-paste
    if (is_type (tree, LT_OPER))
        {
        switch (tree->oper)
        {
        case LTO_PRINT:
            compile_tree (tree->right, f_out);
            fprintf (f_out, "out\n");
            return;
        case LTO_SCAN:
            fprintf (f_out, "in\n");
            return;

        case LTO_PLUS:
            compile_tree (tree->left, f_out);
            compile_tree (tree->right, f_out);
            fprintf (f_out, "add\n");
            return;
        case LTO_MINUS:
            compile_tree (tree->left, f_out);
            compile_tree (tree->right, f_out);
            fprintf (f_out, "sub\n");
            return;
        case LTO_MUL:
            compile_tree (tree->left, f_out);
            compile_tree (tree->right, f_out);
            fprintf (f_out, "mul\n");
            return;
        case LTO_DIV:
            compile_tree (tree->left, f_out);
            compile_tree (tree->right, f_out);
            fprintf (f_out, "div\n");
            return;
        case LTO_POW:
            compile_tree (tree->left, f_out);
            compile_tree (tree->right, f_out);
            fprintf (f_out, "pow\n");
            return;
        case LTO_L:
            compile_tree (tree->left, f_out);
            compile_tree (tree->right, f_out);
            fprintf (f_out, "less\n");
            return;
        case LTO_G:
            compile_tree (tree->left, f_out);
            compile_tree (tree->right, f_out);
            fprintf (f_out, "great\n");
            return;
        case LTO_LEQ:
            compile_tree (tree->left, f_out);
            compile_tree (tree->right, f_out);
            fprintf (f_out, "leq\n");
            return;
        case LTO_GEQ:
            compile_tree (tree->left, f_out);
            compile_tree (tree->right, f_out);
            fprintf (f_out, "geq\n");
            return;
        case LTO_EQ:
            compile_tree (tree->left, f_out);
            compile_tree (tree->right, f_out);
            fprintf (f_out, "eq\n");
            return;
        case LTO_NEQ:
            compile_tree (tree->left, f_out);
            compile_tree (tree->right, f_out);
            fprintf (f_out, "neq\n");
            return;
        case LTO_OR:
            compile_tree (tree->left, f_out);
            compile_tree (tree->right, f_out);
            fprintf (f_out, "or\n");
            return;
        case LTO_AND:
            compile_tree (tree->left, f_out);
            compile_tree (tree->right, f_out);
            fprintf (f_out, "and\n");
            return;
        case LTO_NOT:
            compile_tree (tree->right, f_out);
            fprintf (f_out, "not\n");
            return;
        case LTO_SIN:
            compile_tree (tree->right, f_out);
            fprintf (f_out, "sin\n");
            return;
        case LTO_COS:
            compile_tree (tree->right, f_out);
            fprintf (f_out, "cos\n");
            return;
        case LTO_LN:
            compile_tree (tree->right, f_out);
            fprintf (f_out, "log\n");
            return;
        
        default:
            break;
        }
        }

    if (is_util (tree, LTU_PARAM))
        {
        if (tree->left)
            compile_tree (tree->left, f_out);
        DEB ("Found param of func\n");
        bool is_global = 0;
        get_from_name_table (tree->right->str, 1, &is_global);
        return;
        }

    if (is_util (tree, LTU_WHILE))
        {
        static int while_ind = 0;

        fprintf (f_out, "; starting while %d\n    :begin_of_while_%d\n", while_ind, while_ind);
        compile_tree (tree->left, f_out);
        fprintf (f_out, "   push 0\n    je :end_of_while_%d\n", while_ind);
        compile_tree (tree->right, f_out);
        fprintf (f_out, "    jmp :begin_of_while_%d\n    :end_of_while_%d\n", while_ind, while_ind);

        while_ind++;
        return;
        }

    if (is_util (tree, LTU_IF))
        {
        static int if_ind = 0;

        fprintf (f_out, "; starting if %d\n", if_ind, if_ind);
        compile_tree (tree->left, f_out);
        fprintf (f_out, "   push 0\n    je :false_branch_if_%d\n", if_ind);

        LangTree *dec = tree->right;
        fprintf (f_out, "; true branch of if\n");
        compile_tree (dec->left, f_out);
        fprintf (f_out, "    jmp :end_of_if_%d\n    :false_branch_if_%d\n", if_ind, if_ind);
        if (dec->right)
            compile_tree (dec->right, f_out);
        fprintf(f_out, "    :end_of_if_%d", if_ind);

        if_ind++;
        return;
        }


    if (is_oper (tree, LTO_PRINT))
        {
        compile_tree (tree->right, f_out);
        fprintf (f_out, "out\n");
        return;
        }

    if (tree->left)
        compile_tree (tree->left, f_out);
    if (tree->right)
        compile_tree (tree->right, f_out);
    }

void compile_node_lang (LangTree *tree, FILE *f_out);

void compile_main_branch (LangTree *tree, FILE *f_out)
    {
    assert (tree);
    assert (f_out);

    while (tree)
        {
        if (tree->right && is_util (tree->right, LTU_ASSIGN))
            {
            DEB ("starting reading var\n");
            LangTree *var = tree->right->left;
            char *name = var->str;
            int size = 0;
            if (var->right && is_type (var->right, LT_NUM))
                size = var->right->num;
                
            DEB ("Adding var to globals\n");
            add_to_global_names (name, size + 1);
            }
        else if (tree->right && !is_util (tree->right, LTU_DEF))
            {
            printf ("Unexpected tree at main branch, its type is %c\n", tree->type);
            return;
            }
        tree = tree->left;
        }
    }


/*
while (tree != nullptr)
        {
        if (is_util (tree->right, LTU_ASSIGN))
            {
            compile_assign (tree->right, f_out);
            }
        else if (is_util (tree->right, LTU_DEF))
            {
            // compile function
            LangTree *def = tree->right;
            compile_func_def (def->left, f_out);
            compile_code_block (def->right, f_out);
            clear_local_names ();
            }
        else
            {
            printf ("Unexpected tree at main branch, its type is %c\n", tree->type);
            return;
            }
        tree = tree->left;
        }
*/

