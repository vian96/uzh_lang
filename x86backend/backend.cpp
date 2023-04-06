#include "backend.h"
#include "name_table/name_table.h"
#include "asm_strings.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define is_type(node_, type_) ( (node_)->type == (type_) )
#define is_util(node_, util_) ( ((node_)->type == LT_UTIL) && ((node_)->util == (util_)) )
#define is_oper(node_, oper_) ( ((node_)->type == LT_OPER) && ((node_)->oper == (oper_)) )

#define DEB(...) fprintf (stderr, __VA_ARGS__)

static FILE *TRASH_FILE = NULL;

void compile_lang (LangTree *tree, FILE *f_out)
    {
    assert (tree);
    assert (f_out);

    // TODO more cross-platform solution
    char path_to_temp[4096]; // assuming it is MAX_PATH
    const char* temp = getenv("TEMP");
    assert (temp);
    snprintf(path_to_temp, 4095, "%s\\uzh_trash_file", temp);
    TRASH_FILE = fopen (path_to_temp, "w");

    fprintf (f_out, begin_asm_file);
    
    DEB ("Starting compiling main branch\n");
    init_name_tables ();
    compile_main_branch (tree, f_out);
    DEB ("Ended compiling main branch\n");

    compile_tree (tree, f_out);
    fprintf (f_out, end_asm_file);
    fclose (TRASH_FILE);

    DEB ("Ended compiling all program\n");
    }

void compile_tree (LangTree *tree, FILE *f_out)
    {
    DEB ("Tree type is %c, util is %c\n", tree->type, tree->util);
    if (is_type (tree, LT_NUM))
        {
        DEB ("Compiling num\n");
        fprintf (f_out, "push dword %d\n", tree->num);
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
            fprintf (f_out, "    lea ecx, [ebp - 4*%d]\n", index);
        else
            fprintf (f_out, "    lea ecx, [global_vars + 4*%d]\n", index);
        
        // if this is array
        if (tree->right)
            {
            fprintf (f_out, "    push ecx\n");

            fprintf (f_out, "    ; index\n");
            compile_tree (tree->right, f_out);

            fprintf (f_out, "    pop edx\n    pop ecx\n    lea ecx, [ecx + 4*edx]\n");
            }

        // if (..) is true then write value from stack to this var otherwise put var in stack
        if (tree->parent && is_util (tree->parent, LTU_ASSIGN) && tree->parent->left == tree) 
            fprintf (f_out, "    pop edx\n    mov [ecx], edx\n");
        else
            fprintf (f_out, "    mov edx, [ecx]\n    push edx\n");
        
        return;
        }

    if (is_util (tree, LTU_DEF))
        {
        DEB ("Compiling def\n");
        compile_tree (tree->left, f_out);
        compile_tree (tree->right, TRASH_FILE); // needed to know locals offset

        fprintf (f_out, "push ebp\n"
                        "mov  ebp, esp\n"
                        "sub esp, 4*%d\n", get_local_offset());

        compile_tree (tree->right, f_out);
        if (!strcmp (tree->left->left->str, "main"))
            fprintf (f_out, "\njmp end_of_all_program\n");

        fprintf (f_out, "\nend_of_func_%s:\n\n", tree->left->left->str);
        return;
        }

    if (is_util (tree, LTU_FUNC))
        {
        DEB ("Compiling func\n");
        fprintf (f_out, "\njmp end_of_func_%s\n%s:\n", tree->left->str, tree->left->str);
        clear_local_names ();
        if (tree->right) 
            compile_tree (tree->right, f_out);

        DEB ("Created local names from param\n");
        return;
        }

    if (is_util (tree, LTU_ASSIGN))
        {
        DEB ("Compiling assign\n");
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
            fprintf (f_out, asm_cmd_ "\n");        \
            return;

    if (is_type (tree, LT_OPER))
        {
        DEB ("Compiling oper %c\n", tree->oper);
        switch (tree->oper)
            {
            OPER_TO_ASM (PRINT, "    call Print\n    add esp, 4")
            OPER_TO_ASM (SCAN, "    call Scan\n    push eax")

            OPER_TO_ASM (PLUS, "    pop eax\n    pop edx\n    add eax, edx\n    push eax")
            OPER_TO_ASM (MINUS, "    pop edx\n    pop eax\n    sub eax, edx\n    push eax")
            OPER_TO_ASM (MUL, "    pop edx\n    pop eax\n    mul edx\n    push eax")
            OPER_TO_ASM (DIV, "    xor edx, edx\n    pop ebx\n    pop eax\n"
                                                    "    div ebx\n    push eax")

        #define COMP_ASM(letter) "    xor eax, eax\n    pop ebx\n    pop ecx\n" \
                                 "    cmp ecx, ebx\n    set"#letter" al\n    push eax"
            OPER_TO_ASM (L, COMP_ASM(l))
            OPER_TO_ASM (G, COMP_ASM(g))
            OPER_TO_ASM (LEQ, COMP_ASM(le))
            OPER_TO_ASM (GEQ, COMP_ASM(ge))
            OPER_TO_ASM (EQ, COMP_ASM(e))
            OPER_TO_ASM (NEQ, COMP_ASM(ne))
        #undef COMP_ASM

            // TODO it is bitwise, not logical
            OPER_TO_ASM (OR, "    pop eax\n    pop edx\n    or eax, edx\n push eax")
            OPER_TO_ASM (AND, "    pop eax\n    pop edx\n    and eax, edx\n push eax")
            OPER_TO_ASM (NOT, "    pop eax\n    not eax\n push eax")

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
        get_from_name_table (tree->right->str, 1, &is_global, true);
        return;
        }

    if (is_util (tree, LTU_WHILE))
        {
        static int while_ind = 0;
        int cur_while_ind = while_ind;
        while_ind++;

        fprintf (f_out, "; starting while %d\n    begin_of_while_%d:\n", 
                        cur_while_ind, cur_while_ind);
        
        // condition
        compile_tree (tree->left, f_out); 
        fprintf (f_out, "   pop eax\n    cmp eax, 0\n    je end_of_while_%d\n", 
                        cur_while_ind);

        compile_tree (tree->right, f_out);
        fprintf (f_out, "    jmp begin_of_while_%d\n    end_of_while_%d:\n", 
                        cur_while_ind, cur_while_ind);

        return;
        }

    if (is_util (tree, LTU_CALL))
        {
        DEB ("FOUND CALL, starting params\n");

        int param_cnt = 0;

        LangTree *param = tree->right;
        while (param)
            {
            param_cnt++;
            fprintf (f_out, "\n; Calcing params\n");
            compile_tree (param->right, f_out);
            if (param->left == nullptr || is_type (param->left, LT_EMPTY))
                break;
            param = param->left;
            assert (param->parent->left == param);
            }

        // calling function, it leaves ret in eax
        fprintf (f_out, "call %s\n\n", tree->left->str);
        fprintf (f_out, "add esp, 4*%d\n", param_cnt);
        fprintf (f_out, "push eax\n");

        return;
        }

    if (is_util (tree, LTU_RET))
        {
        compile_tree (tree->right, f_out);
        fprintf (f_out, "pop eax\n"
                        "mov esp, ebp\n"
                        "pop ebp\n"
                        "ret\n");
        return;
        }

    if (is_util (tree, LTU_IF))
        {
        static int if_ind = 0;
        int cur_if_ind = if_ind;
        if_ind++;

        fprintf (f_out, "; starting if %d\n", cur_if_ind);
        compile_tree (tree->left, f_out);
        fprintf (f_out, "   pop eax\n    cmp eax, 0\n    je false_branch_if_%d\n", cur_if_ind);

        LangTree *dec = tree->right;
        fprintf (f_out, "; true branch of if\n");
        compile_tree (dec->left, f_out);
        fprintf (f_out, "    jmp end_of_if_%d\n    false_branch_if_%d:\n", cur_if_ind, cur_if_ind);
        if (dec->right)
            compile_tree (dec->right, f_out);
        fprintf(f_out, "    end_of_if_%d:\n", cur_if_ind);

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


