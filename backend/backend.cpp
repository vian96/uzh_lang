#include "backend.h"
#include "name_table/name_table.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

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
             "; register bx is used for pointing to begin of local vars\n"
             "; register cx is used for temporary using of var address\n"
             "; register dx is used for returning values from func\n\n");
    
    DEB ("Starting compiling main branch\n");
    init_name_tables ();
    compile_main_branch (tree, f_out);
    DEB ("Ended compiling main branch\n");

    int glob_offset = get_global_offset ();
    fprintf (f_out, "push %d\npop bx\n", glob_offset);
    
    compile_tree (tree, f_out);
    fprintf (f_out, "\n\njmp :main\n:end_of_all_program\n");
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
            fprintf (f_out, "    pop [cx]\n");
        else
            fprintf (f_out, "    push [cx]\n");
        
        return;
        }

    if (is_util (tree, LTU_FUNC))
        {
        DEB ("Compiling func\n");
        fprintf (f_out, "\njmp :end_of_func_%s\n:%s\n", tree->left->str, tree->left->str);
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
        if (!strcmp (tree->left->left->str, "main"))
            fprintf (f_out, "\njmp :end_of_all_program\n");
        fprintf (f_out, "\n:end_of_func_%s\n\n", tree->left->left->str);
        return;
        }

    if (is_util (tree, LTU_ASSIGN))
        {
        DEB ("Compiling assing\n");
        compile_tree (tree->right, f_out);
        compile_tree (tree->left, f_out);
        return;
        }

#define OPER_TO_ASM(oper_, asm_cmd_)                \
    case LTO_##oper_:                               \
            if (tree->left)                         \
                compile_tree (tree->left, f_out);   \
            if (tree->right)                        \
                compile_tree (tree->right, f_out);  \
            fprintf (f_out, #asm_cmd_ "\n");        \
            return;

    if (is_type (tree, LT_OPER))
        {
        DEB ("Compiling oper %c\n", tree->oper);
        switch (tree->oper)
            {
            OPER_TO_ASM (PRINT, out)
            OPER_TO_ASM (GRAPH, print)
            OPER_TO_ASM (SCAN, in)

            OPER_TO_ASM (PLUS, add)
            OPER_TO_ASM (MINUS, sub)
            OPER_TO_ASM (MUL, mul)
            OPER_TO_ASM (DIV, div)
            OPER_TO_ASM (POW, pow)

            OPER_TO_ASM (L, less)
            OPER_TO_ASM (G, great)
            OPER_TO_ASM (LEQ, leq)
            OPER_TO_ASM (GEQ, geq)
            OPER_TO_ASM (EQ, eq)
            OPER_TO_ASM (NEQ, neq)

            OPER_TO_ASM (OR, or)
            OPER_TO_ASM (AND, and)
            OPER_TO_ASM (NOT, not)

            OPER_TO_ASM (SIN, sin)
            OPER_TO_ASM (COS, cos)
            OPER_TO_ASM (LN, log)
            OPER_TO_ASM (SQRT, sqrt)

            default:
                break;
            }
        }

#undef OPER_TO_ASM

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
        int cur_while_ind = while_ind;
        while_ind++;

        fprintf (f_out, "; starting while %d\n    :begin_of_while_%d\n", cur_while_ind, cur_while_ind);
        compile_tree (tree->left, f_out);
        fprintf (f_out, "   push 0\n    je :end_of_while_%d\n", cur_while_ind);
        compile_tree (tree->right, f_out);
        fprintf (f_out, "    jmp :begin_of_while_%d\n    :end_of_while_%d\n", cur_while_ind, cur_while_ind);

        return;
        }

    if (is_util (tree, LTU_CALL))
        {
        DEB ("FOUND CALL, starting params\n");

        LangTree *param = tree->right;
        while (param)
            {
            fprintf (f_out, "\n; Calcing params\n");
            compile_tree (param->right, f_out);
            if (param->left == nullptr || is_type (param->left, LT_EMPTY))
                break;
            param = param->left;
            assert (param->parent->left == param);
            }

        int offset = get_local_offset ();
        fprintf (f_out, "; calling function \n"
                        "    push bx\n    push %d\n"
                        "    add    \n    pop bx\n", offset);
        
        int cnt = 0;
        if (param)
            while (param != tree)
                {
                DEB ("Reading params\n");
                fprintf (f_out, "  push bx\n  push %d\n  add\n  pop cx\n  pop [cx]\n", cnt);
                cnt++;
                param = param->parent;
                }
        
        fprintf (f_out, "call :%s\npush dx\n\n", tree->left->str);
        fprintf (f_out, "    push bx\n    push %d\n"
                        "    add    \n    pop bx\n", -offset);
        return;
        }

    if (is_util (tree, LTU_RET))
        {
        compile_tree (tree->right, f_out);
        fprintf (f_out, "    pop dx\n    ret\n");
        return;
        }

    if (is_util (tree, LTU_IF))
        {
        static int if_ind = 0;
        int cur_if_ind = if_ind;
        if_ind++;

        fprintf (f_out, "; starting if %d\n", cur_if_ind);
        compile_tree (tree->left, f_out);
        fprintf (f_out, "   push 0\n    je :false_branch_if_%d\n", cur_if_ind);

        LangTree *dec = tree->right;
        fprintf (f_out, "; true branch of if\n");
        compile_tree (dec->left, f_out);
        fprintf (f_out, "    jmp :end_of_if_%d\n    :false_branch_if_%d\n", cur_if_ind, cur_if_ind);
        if (dec->right)
            compile_tree (dec->right, f_out);
        fprintf(f_out, "    :end_of_if_%d\n", cur_if_ind);

        return;
        }

    if (tree->left)
        compile_tree (tree->left, f_out);
    if (tree->right)
        compile_tree (tree->right, f_out);
    }

void compile_main_branch (LangTree *tree, FILE *f_out)
    {
    assert (tree);
    assert (f_out);

    if (tree->left)
        compile_main_branch (tree->left, f_out);

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
    }

#undef is_type
#undef is_util
#undef is_oper


