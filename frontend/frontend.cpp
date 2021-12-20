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
            free_tree_lang (tree);
            return nullptr;
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
            func->right = read_param (code, func);
        
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

LangTree *read_param (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);
    DEB ("Starting params\n");
    print_lexem_line (*code);
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

    param->parent = parent;
    return param;
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
                break;
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
            break;

        DEB ("Read sth\n");

        tree->parent->left = tree;
        tree = tree->parent;
        }

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
    
    if (!isalpha (lex->str[0]))
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
        // TODO move to function read_call
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

// TODO is it okay to have these two (if and while) similar functions
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
                    DEB ("Success if with else\n");
                    return tree;
                    }
                DEB ("Didn't found : after else\n");
                break;
                }
            else
                lex--; // to avoid reading spaces when there is no 'else'
            }
        DEB ("Success if\n");
        return tree;
    } while (0);

    free_tree_lang (tree);
    *code = start;
    return nullptr;
    }

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

#define BEG_BIN(cur_nam_, child_nam_)   \
LangTree *read_##cur_nam_ (Lexem **code, LangTree *parent)   \
    {                                                       \
    assert (code);                                          \
    assert (*code);                                         \
    Lexem *start = *code;                                   \
    LangTree *tree = read_##child_nam_ (code, parent);      \
    if (!tree)                                              \
        return nullptr;                                     \
    do {                                                    \
        while (1)                                           \
            {                                               

#define GEN_BIN(cur_op_, cur_str_)              \
            if (is_sym (#cur_str_))                  \
                tree->parent = new_lt_op (LTO_##cur_op_, parent); \
            else

#define END_BIN(cur_nam_, child_nam_)                     \
                return tree;                    \
            lex++;                              \
            tree->parent->left = tree;          \
            tree = tree->parent;                \
            tree->right = read_##child_nam_ (code, tree); \
            if (!tree->right)                   \
                break;                          \
            }                                   \
    } while (0);                                \
    free_tree_lang (tree);                      \
    *code = start;                              \
    return nullptr;                             \
    }

BEG_BIN (and, eq)
    GEN_BIN (AND, &&)
END_BIN (and, eq)

BEG_BIN (eq, comp)
    GEN_BIN (EQ, ==)
    GEN_BIN (NEQ, !=)
END_BIN (eq, comp)

BEG_BIN (comp, sum)
    GEN_BIN (L, <)
    GEN_BIN (G, >)
    GEN_BIN (LEQ, <=)
    GEN_BIN (GEQ, >=)
END_BIN (comp, sum)

BEG_BIN (sum, mul)
    GEN_BIN (PLUS, +)
    GEN_BIN (MINUS, -)
END_BIN (sum, mul)

BEG_BIN (mul, pow)
    GEN_BIN (MUL, *)
    GEN_BIN (DIV, /)
END_BIN (mul, pow)

BEG_BIN (pow, neg)
    GEN_BIN (POW, ^)
END_BIN (pow, neg)

#undef BEG_BIN                                            
#undef GEN_BIN
#undef END_BIN

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
    else if (is_id ("graph"))
        {
        lex++;
        return new_lt_op (LTO_GRAPH, parent);
        }
    else
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


#undef lex
#undef is_sym
#undef is_id
#undef is_space
