#include "frontend.h"

#include <assert.h>
#include <stdio.h>
#include <ctype.h>

#define DEB(...) printf (__VA_ARGS__)
// #define DEB(...)

LangTree *lang_frontend (Lexem *code)
    {
    assert (code);
    DEB ("Starting frontend\n");
    print_lexem_line (code);

    LangTree *tree = new_lt_empty ();
    while (code[1].type != LEX_TERM)
        {
        code++; // to skip LEX_SPACE
        tree->parent = read_func (&code, nullptr);
        if (!tree->parent)
            tree->parent = read_statement (&code, nullptr);
        if (tree->parent == nullptr)
            {
            printf ("Error at code\n");
            // TODO if it returns tree, change it to return null and free
            return tree;
            }
        DEB ("Read part!!\n");
        tree->parent->left = tree;
        tree = tree->parent;
        }
    return tree;
    }

#define lex (*code)
#define is_sym(sym_) ( (lex->type == LEX_SYM) && (!strcmp (lex->str, (sym_))) )
#define is_id(id_) ( (lex->type == LEX_ID) && (!strcmp (lex->str, (id_))) )
#define is_space(n_) ( (lex->type == LEX_SPACE) && (lex->cnt == (n_)) )

LangTree *read_func (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);
    DEB ("Starting func\n");
    print_lexem_line (*code);

    Lexem *start = *code;
    LangTree *tree = new_lt_util (LTU_STM, parent);
    tree->right = new_lt_util (LTU_DEF, tree);
    tree->right->left = new_lt_util (LTU_FUNC, tree->right);
    LangTree *func = tree->right->left;
    
    DEB ("Did setup\n");
    
    // TODO move read_param to another func
    // TODO is this situation good enough for goto?
    do {
        // printf 
        if (!is_id ("def"))
            break;
        lex++;
        DEB ("Seen def\n");
        
        if (lex->type != LEX_ID)
            break;
        func->left = new_lt_id (lex->str, func);
        lex++;
        DEB ("Seen id\n");

        if (!is_sym ("("))
            break;
        lex++;
        DEB ("Seen (\n");

        if (lex->type == LEX_ID)
            {
            LangTree *param = new_lt_empty ();
            while (lex->type == LEX_ID)
                {
                param->parent = new_lt_util (LTU_PARAM);
                param->parent->left = param;
                param = param->parent;
                
                param->right = new_lt_id (lex->str, param);
                lex++;
                if (!is_sym (","))
                    break;
                lex++;
                }
            func->right = param;
            param->parent = func;
            }
        
        if (!is_sym (")"))
            break;
        lex++;
        DEB ("Seen )\n");

        if (!is_sym (":"))
            break;
        lex++;
        DEB ("Seen :\n");

        tree->right->right = read_block (code, 4, tree->right);
        if (!tree->right->right)
            break;

        return tree;
    } while (0);

    free_tree_lang (tree);
    *code = start;
    return nullptr;
    }

LangTree *read_statement (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);
    DEB ("Starting statement\n");
    print_lexem_line (*code);

    Lexem *start = *code;
    LangTree *tree = new_lt_util (LTU_STM, parent);

    do {
        tree->right = read_return (code, tree);
        if (!tree->right)
            tree->right = read_assign (code, tree);
        if (!tree->right)
            tree->right = read_expr (code, tree);
        if (!tree->right)
            break;
        DEB ("Success statement\n");

        return tree;
    } while (0);

    free_tree_lang (tree);
    *code = start;
    return nullptr;
    }

// TODO i WANT goto, but is it okay? it is try-catch way
LangTree *read_block (Lexem **code, int n, LangTree *parent)
    {
    assert (code);
    assert (*code);
    DEB ("Start block\n");
    print_lexem_line (*code);

    DEB ("trying space\n");
    if (!is_space (n))
        return nullptr;
    DEB ("Seen space\n");

    Lexem *start = *code;
    LangTree *tree = new_lt_empty ();

    while (1) 
        {
        if (!is_space (n))
            {            
            if (lex->cnt > n)
                {
                printf ("CHCK %d %d\n", lex->type, lex->cnt);
                printf ("Indentation error, expected: %d or %d, got: %d\n", n, n-4, lex->cnt);
                goto fail_read_block_free;
                }
            DEB ("Success block\n");
            tree->parent = parent;
            return tree;
            }
        lex++;
        DEB ("Seen loop space\n");

        tree->parent = read_if (code, n, nullptr);
        if (!tree->parent)
            tree->parent = read_while (code, n, nullptr);
        if (!tree->parent)
            tree->parent = read_statement (code, nullptr);
        if (!tree->parent)
            goto fail_read_block_free;

        DEB ("Read sth\n");

        tree->parent->left = tree;
        tree = tree->parent;
        }

    fail_read_block_free:
    free_tree_lang (tree);
    *code = start;
    DEB ("FAIL block\n");
    return nullptr;
    }

LangTree *read_assign (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);

    Lexem *start = *code;
    LangTree *tree = new_lt_util (LTU_ASSIGN, parent);

    do {
        tree->left = read_var (code, tree);
        if (!tree->left)
            break;
        
        if (!is_sym ("="))
            break;
        lex++;
        
        tree->right = read_expr (code, tree);
        if (!tree->right)
            break;
        
        return tree;
    } while (0);

    free_tree_lang (tree);
    *code = start;
    return nullptr;
    }

LangTree *read_var (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);

    printf ("Anylyzing var\n");
    print_lexem_line (lex);

    if (lex->type == LEX_NUM)
        {
        LangTree *tree = new_lt_num (lex->number, parent);
        lex++;
        return tree;
        }

    if (is_sym ("("))
        {
        // expression
        printf ("Entered () in var\n");
        print_lexem_line (lex);
        lex++;
        LangTree *tree = read_expr (code, parent);
        if (!tree)
            {
            printf ("Error reading expression in ()\n");
            free_tree_lang (tree);
            return nullptr;
            }
        if (!is_sym (")"))
            {
            printf ("No closed ) after expression in ()\n");
            print_lexem_line (lex);
            free_tree_lang (tree);
            return nullptr;
            }
        lex++; 
        return tree;
        }

    if (lex->type != LEX_ID)
        return nullptr;
    
    if (!isalnum (lex->str[0]))
        return nullptr;

    LangTree *tree = new_lt_id (lex->str, parent);
    lex++;
    if (is_sym ("["))
        {
        lex++;
        tree->right = read_expr (code, tree);
        if (!tree->right)
            {
            printf ("Failed to read expression in []\n");
            free_tree_lang (tree);
            return nullptr;
            }
        if (!is_sym ("]"))
            {
            printf ("No ] after expression\n");
            free_tree_lang (tree);
            return nullptr;
            }
        lex++;
        }
    else if (is_sym ("("))
        {
        lex++;
        DEB ("Found function call\n");
        print_lexem_line (lex);

        LangTree *tmp = new_lt_util (LTU_CALL, parent);
        tmp->left = tree;
        tree = tmp;
        
        if (!is_sym (")"))
            {
            LangTree *param = new_lt_empty ();
            while (!is_sym (")"))
                {
                printf ("Begin of call param loop\n");
                param->parent = new_lt_util (LTU_PARAM);
                param->parent->left = param;
                param = param->parent;
                
                param->right = read_expr (code, param);
                if (!param->right)
                    {
                    printf ("Bad expression in params of call\n");
                    return nullptr;
                    }
                DEB ("Read in-call expression\n");
                print_lexem_line (lex);
                if (!is_sym (","))
                    break;
                lex++;
                }
            tree->right = param;
            param->parent = tree;
            }
        
        if (!is_sym (")"))
            {
            printf ("Error, no closing ) after function call\n");
            // TODO here should be free and nullptr
            return tree;
            }        
        lex++;
        DEB ("Succes function call\n");
        print_lexem_line (lex);
        }
    return tree;
    }

// WARNING this is copy-paste of read_while so be accurate when changing!!!!
LangTree *read_if (Lexem **code, int n, LangTree *parent)
    {
    assert (code);
    assert (*code);
    DEB ("Starting if\n");

    Lexem *start = *code;
    LangTree *tree = new_lt_util (LTU_STM, parent);
    tree->right = new_lt_util (LTU_IF, tree);
    LangTree *if_tree = tree->right;

    do {
        if (!is_id ("if"))
            break;
        lex++;
        DEB ("Seen if\n");

        if_tree->left = read_expr (code, if_tree);
        if (!if_tree->left)
            break;
        DEB ("Seen cond\n");

        if (!is_sym (":"))
            break;
        lex++;
        DEB ("Seen :\n");
        
        if_tree->right = new_lt_util (LTU_DECIS, if_tree);
        LangTree *decis = if_tree->right;
        
        decis->left = read_block (code, n + 4, decis);
        if (!decis->left)
            break;
        DEB ("Seen block\n");

        if (is_space (n))
            {
            lex++;
            if (is_id ("else"))
                {
                lex++;
                if (is_sym (":"))
                    {
                    DEB ("Found legit else\n");
                    lex++;
                    decis->right = read_block (code, n + 4, decis);
                    if (!decis->right)
                        break;
                    return tree;
                    }
                DEB ("Didn't found : after else\n");
                break;
                }
            lex--;
            }
        DEB ("Success if\n");
        return tree;
    } while (0);

    free_tree_lang (tree);
    *code = start;
    return nullptr;
    }

// WARNING this is copy-paste of read_if so be accurate when changing!!!!
LangTree *read_while (Lexem **code, int n, LangTree *parent)
    {
    assert (code);
    assert (*code);
    DEB ("Starting while\n");

    Lexem *start = *code;
    LangTree *tree = new_lt_util (LTU_STM, parent);
    tree->right = new_lt_util (LTU_WHILE, tree);
    LangTree *whil = tree->right;

    do {
        if (!is_id ("while"))
            break;
        lex++;
        DEB ("Seen while\n");

        whil->left = read_expr (code, whil);
        if (!whil->left)
            break;
        DEB ("Seen cond\n");

        if (!is_sym (":"))
            break;
        lex++;
        DEB ("Seen :\n");
        
        whil->right = read_block (code, n + 4, whil);
        if (!whil->right)
            break;
        DEB ("Seen block\n");

        return tree;
    } while (0);

    free_tree_lang (tree);
    *code = start;
    return nullptr;
    }

LangTree *read_return (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);

    Lexem *start = *code;
    LangTree *tree = new_lt_util (LTU_RET, parent);

    do {
        if (!is_id ("return"))
            break;
        lex++;

        tree->right = read_expr (code, tree);
        if (!tree->right)
            break;

        return tree;
    } while (0);

    free_tree_lang (tree);
    *code = start;
    return nullptr;
    }

// TODO remove copy-paste of functions
LangTree *read_expr (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);
    DEB ("Got in expr\n");
    print_lexem_line (lex);

    Lexem *start = *code;
    LangTree *tree = read_and (code, parent);
    if (!tree)
        return nullptr;
    DEB ("Read first and\n");

    do {
        while (1)
            {
            if (!is_sym ("||"))
                return tree;
            lex++;
            DEB ("Continue expr\n");
            tree->parent = new_lt_op (LTO_OR, parent);
            tree->parent->left = tree;
            tree = tree->parent;
            tree->right = read_and (code, tree);
            if (!tree->right)
                break;
            }
    } while (0);

    DEB ("Expr failed :(\n");
    free_tree_lang (tree);
    *code = start;
    return nullptr;
    }

LangTree *read_and (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);

    Lexem *start = *code;
    LangTree *tree = read_eq (code, parent);
    if (!tree)
        return nullptr;

    do {
        while (1)
            {
            if (!is_sym ("&&"))
                return tree;
            lex++;
            tree->parent = new_lt_op (LTO_AND, parent);
            tree->parent->left = tree;
            tree = tree->parent;
            tree->right = read_eq (code, tree);
            if (!tree->right)
                break;
            }
    } while (0);

    free_tree_lang (tree);
    *code = start;
    return nullptr;
    }

LangTree *read_eq (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);

    Lexem *start = *code;
    LangTree *tree = read_comp (code, parent);
    if (!tree)
        return nullptr;

    do {
        while (1)
            {
            if (is_sym ("=="))
                tree->parent = new_lt_op (LTO_EQ, parent);
            else if (is_sym ("!="))
                tree->parent = new_lt_op (LTO_NEQ, parent);
            else 
                return tree;
            lex++;
            tree->parent->left = tree;
            tree = tree->parent;
            tree->right = read_comp (code, tree);
            if (!tree->right)
                break;
            }
    } while (0);

    free_tree_lang (tree);
    *code = start;
    return nullptr;
    }

LangTree *read_comp (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);

    Lexem *start = *code;
    LangTree *tree = read_sum (code, parent);
    if (!tree)
        return nullptr;

    do {
        while (1)
            {
            if (is_sym ("<"))
                tree->parent = new_lt_op (LTO_L, parent);
            else if (is_sym (">"))
                tree->parent = new_lt_op (LTO_G, parent);
            else if (is_sym ("<="))
                tree->parent = new_lt_op (LTO_LEQ, parent);
            else if (is_sym (">="))
                tree->parent = new_lt_op (LTO_GEQ, parent);
            else
                return tree;
            lex++;
            tree->parent->left = tree;
            tree = tree->parent;
            tree->right = read_sum (code, tree);
            if (!tree->right)
                break;
            }
    } while (0);

    free_tree_lang (tree);
    *code = start;
    return nullptr;
    }

LangTree *read_sum (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);

    Lexem *start = *code;
    LangTree *tree = read_mul (code, parent);
    if (!tree)
        return nullptr;

    do {
        while (1)
            {
            if (is_sym ("+"))
                tree->parent = new_lt_op (LTO_PLUS, parent);
            else if (is_sym ("-"))
                tree->parent = new_lt_op (LTO_MINUS, parent);
            else
                return tree;
            lex++;
            tree->parent->left = tree;
            tree = tree->parent;
            tree->right = read_mul (code, tree);
            if (!tree->right)
                break;
            }
    } while (0);

    free_tree_lang (tree);
    *code = start;
    return nullptr;
    }
    
LangTree *read_mul (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);

    Lexem *start = *code;
    LangTree *tree = read_pow (code, parent);
    if (!tree)
        return nullptr;

    do {
        while (1)
            {
            if (is_sym ("*"))
                tree->parent = new_lt_op (LTO_MUL, parent);
            else if (is_sym ("/"))
                tree->parent = new_lt_op (LTO_DIV, parent);
            else
                return tree;
            lex++;
            tree->parent->left = tree;
            tree = tree->parent;
            tree->right = read_pow (code, tree);
            if (!tree->right)
                break;
            }
    } while (0);

    free_tree_lang (tree);
    *code = start;
    return nullptr;
    }

LangTree *read_pow (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);

    Lexem *start = *code;
    LangTree *tree = read_neg (code, parent);
    if (!tree)
        return nullptr;

    do {
        while (1)
            {
            if (is_sym ("^"))
                tree->parent = new_lt_op (LTO_POW, parent);
            else
                return tree;
            lex++;
            tree->parent->left = tree;
            tree = tree->parent;
            tree->right = read_neg (code, tree);
            if (!tree->right)
                break;
            }
    } while (0);

    free_tree_lang (tree);
    *code = start;
    return nullptr;
    }

LangTree *read_neg (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);

    if (is_sym ("!"))
        {
        lex++;
        LangTree *tree = new_lt_op (LTO_NOT, parent);
        tree->right = read_brack (code, parent);
        if (!tree->right)
            {
            free_tree_lang (tree);
            lex--;
            return nullptr;
            }
        return tree;
        }
    return read_brack (code, parent);
    }

LangTree *read_brack (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);

    LangTree *tree = nullptr; // new_lt_op (LTO_NOT, parent);
    if (is_id ("sin"))
        tree = new_lt_op (LTO_SIN, parent);
    else if (is_id ("cos"))
        tree = new_lt_op (LTO_COS, parent);
    else if (is_id ("ln"))
        tree = new_lt_op (LTO_LN, parent);
    else if (is_id ("sqrt"))
        tree = new_lt_op (LTO_SQRT, parent);
    else if (is_id ("print"))
        tree = new_lt_op (LTO_PRINT, parent);
    // it is the way it should be, scan does not have any childs
    else if (is_id ("input"))
        {
        lex++;
        return new_lt_op (LTO_SCAN, parent);
        }
    
    if (!tree)
        return read_var (code, parent);
        
    lex++;
    tree->right = read_var (code, tree);
    if (!tree->right)
        {
        free_tree_lang (tree);
        lex--;
        return nullptr;
        }
    return tree;
    }

/* 
Use as template
LangTree *read_ (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);

    Lexem *start = *code;
    LangTree *tree = new_lt_empty ();

    do {

    } while (0);

    free_tree_lang (tree);
    *code = start;
    return nullptr;
    }
*/

