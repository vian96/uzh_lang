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
    print_lexems (code);

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
    print_lexems (*code);

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
                
                param->right = new_lt_id (lex->str);
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
    print_lexems (*code);

    Lexem *start = *code;
    LangTree *tree = new_lt_util (LTU_STM);

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
    print_lexems (*code);

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
            
            if (!is_space (n-4))
            //if (lex->cnt != n-4 || lex->type != LEX_SPACE)
                {
                printf ("CHCK %d %d\n", lex->type, lex->cnt);
                printf ("Indentation error, expected: %d or %d, got: %d\n", n, n-4, lex->cnt);
                goto fail_read_block_free;
                }
            DEB ("Success block\n");
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

LangTree *read_expr (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);

    return read_var (code, parent);
    }

LangTree *read_var (Lexem **code, LangTree *parent)
    {
    assert (code);
    assert (*code);

    if (lex->type != LEX_ID)
        return nullptr;
    
    if (!isalnum (lex->str[0]))
        return nullptr;

    LangTree *tree = new_lt_id (lex->str, parent);
    lex++;
    return tree;
    }

LangTree *read_while (Lexem **code, int n, LangTree *parent)
    {
    return nullptr;
    }

LangTree *read_if (Lexem **code, int n, LangTree *parent)
    {
    return nullptr;
    }

LangTree *read_return (Lexem **code, LangTree *parent)
    {
    return nullptr;
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

