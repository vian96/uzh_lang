#include "lang_tree.h"
#include "../lexer/lexer.h"

#include <asm/asm/string_utils.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define DEB(...) printf (__VA_ARGS__)
// #define DEB(...) 

void lang_tree_lang_ctor (LangTree *tree, LangTreeType type, LTNum num, char *str, 
                     LangTreeOper oper, LangTreeUtil util, LangTree *left, LangTree *right)
    {
    assert (tree);

    tree->type = type;
    tree->num = num;
    tree->str = str;
    tree->oper = oper;
    tree->util = util;
    tree->left = left;
    tree->right = right;
    }

void free_tree_lang (LangTree *tree)
    {
    assert (tree);

    if (tree->left)
        free_tree_lang (tree->left);    
    if (tree->right)
        free_tree_lang (tree->right);
    free (tree);
    }

LangTree *new_lang_tree (LangTreeType type, LTNum num, char *str, 
                     LangTreeOper oper, LangTreeUtil util, LangTree *left, LangTree *right)
    {
    LangTree *tree = (LangTree*) calloc (1, sizeof (*tree));
    lang_tree_lang_ctor (tree, type, num, str, oper, util, left, right);
    return tree;
    }
    
LangTree *new_lt_op (LangTreeOper oper, LangTree *parent, LangTree *left, LangTree *right)
    {
    LangTree *to_ret = new_lang_tree (LT_OPER, parent, 0, nullptr, oper);
    to_ret->left = left;
    to_ret->left = right;
    return to_ret;
    }

LangTree *new_lt_util (LangTreeUtil util, LangTree *parent, LangTree *left, LangTree *right)
    {
    LangTree *to_ret = new_lang_tree (LT_UTIL, parent, 0, nullptr, LTO_INVALID, util);
    to_ret->left = left;
    to_ret->left = right;
    return to_ret;
    }

LangTree *new_lt_num (LTNum num, LangTree *parent, LangTree *left, LangTree *right)
    {
    LangTree *to_ret = new_lang_tree (LT_NUM, parent, num);
    to_ret->left = left;
    to_ret->left = right;
    return to_ret;
    }

LangTree *new_lt_id (char *name, LangTree *parent, LangTree *left, LangTree *right)
    {
    LangTree *to_ret = new_lang_tree (LT_NUM, parent, 0, strdup (name));
    to_ret->left = left;
    to_ret->left = right;
    return to_ret;
    }

LangTree *read_lang_tree (const char **str, LangTree *parent)
    {
    assert (str);

    if (**str != '(')
        {
        printf ("ERROR: you passed to read_tree string without '(' in begin, it is: %s\n", *str);
        return nullptr;
        }
    (*str)++;

    LangTree * const tree = new_lang_tree (LT_INVALID, parent);

    if (**str == '(')
        {
        DEB ("Found left child\n");
        tree->left = read_lang_tree (str, tree);
        (*str)++;   // because it will point to ')' after search
        }

    // TODO change if change type of num
    int num = 0;
    LangTreeOper oper = LTO_INVALID;
    LangTreeUtil util = LTU_INVALID;
    if (sscanf (*str, "%d", &num))
        {
        // found a number
        DEB ("Found a number\n");
 
        *str = strchr (*str, ')');
        (*str)++; // because it will point to ')' after search

        tree->type = LT_NUM;
        tree->num = num;

        return tree;
        }
    else if (**str == '\'')
        {
        DEB ("Found an identifier\n");
        (*str)++;
        tree->type = LT_ID;
        tree->str = copy_str_if (str, isalnum);
        (*str)++; // because points at '
        }
    else if (oper = get_lt_oper (str))
        {
        DEB ("Found an oper %c\n", oper);
        tree->type = LT_OPER;
        tree->oper = oper;
        }
    else if (util = get_lt_util (str))
        {
        DEB ("Found an util %c\n", util);
        tree->type = LT_UTIL;
        tree->util = util;
        }
    else 
        {
        printf ("Error: unknown type of tree while parsing file with it\n");
        return nullptr;
        }

    if (**str == '(')
        {
        DEB ("Found right child\n");
        tree->right = read_lang_tree (str, tree);
        (*str)++;   // because it will point to ')' after search
        }

    return tree;
    }

// TODO is there any way to reduce copy-paste?
LangTreeOper get_lt_oper (const char **str)
    {
    assert (str);
    assert (*str);

    LangTreeOper oper = LTO_INVALID;
    switch (**str)
    {
    case '+':
    case '-':
    case '*':
    case '/':
    case '^':
    case '&':
    case '|':
        oper = (LangTreeOper) (**str);
        (*str)++;
        return oper;
        
    case '<':
        (*str)++;
        if (**str == '=')
            {
            (*str)++;
            return LTO_LEQ;
            }
        return LTO_L;

    case '>':
        (*str)++;
        if (**str == '=')
            {
            (*str)++;
            return LTO_GEQ;
            }
        return LTO_G;

    case '=':
        if ((*str)[1] == '=')
            {
            (*str) += 2;
            return LTO_EQ;
            }
        return LTO_INVALID;

    case '!':
        if ((*str)[1] == '=')
            {
            (*str) += 2;
            return LTO_NEQ;
            }
        return LTO_NOT;
    
    default:
        break;
    }

    if (is_equal_words (*str, "sin"))
        {
        (*str) += 3;
        return LTO_SIN;
        }
    if (is_equal_words (*str, "cos"))
        {
        (*str) += 3;
        return LTO_COS;
        }
    if (is_equal_words (*str, "ln"))
        {
        (*str) += 2;
        return LTO_LN;
        }

    if (is_equal_words (*str, "print"))
        {
        (*str) += 5;
        return LTO_SIN;
        }
    if (is_equal_words (*str, "scan"))
        {
        (*str) += 4;
        return LTO_SIN;
        }
    
    return LTO_INVALID;
    }

LangTreeUtil get_lt_util (const char **str)
    {
    // WARNING: this is important to call this function
    // ONLY AFTER GET_OPER becase it may fail at '=='
    assert (str);
    assert (*str);

    if (**str == '=')
        {
        (*str)++;
        return LTU_ASSIGN;
        }

    if (is_equal_words (*str, "if"))
        {
        (*str) += 2;
        return LTU_IF;
        }
    if (is_equal_words (*str, "decision"))
        {
        (*str) += 8;
        return LTU_DECIS;
        }
    if (is_equal_words (*str, "while"))
        {
        (*str) += 5;
        return LTU_WHILE;
        }
    if (is_equal_words (*str, "return"))
        {
        (*str) += 6;
        return LTU_RET;
        }
    if (is_equal_words (*str, "define"))
        {
        (*str) += 6;
        return LTU_DEF;
        }
    if (is_equal_words (*str, "call"))
        {
        (*str) += 4;
        return LTU_CALL;
        }
    if (is_equal_words (*str, "function"))
        {
        (*str) += 8;
        return LTU_FUNC;
        }
    if (is_equal_words (*str, "parameter"))
        {
        (*str) += 9;
        return LTU_PARAM;
        }
    if (is_equal_words (*str, "statement"))
        {
        (*str) += 9;
        return LTU_STM;
        }
        
    return LTU_INVALID;
    }

