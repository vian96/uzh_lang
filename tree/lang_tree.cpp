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

void lang_tree_lang_ctor (LangTree *tree, LangTree *parent, LangTreeType type, LTNum num, char *str, 
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
    tree->parent = parent;
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

LangTree *new_lang_tree (LangTreeType type, LangTree *parent, LTNum num, char *str, 
                     LangTreeOper oper, LangTreeUtil util, LangTree *left, LangTree *right)
    {
    LangTree *tree = (LangTree*) calloc (1, sizeof (*tree));
    lang_tree_lang_ctor (tree, parent, type, num, str, oper, util, left, right);
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
    LangTree *to_ret = new_lang_tree (LT_ID, parent, 0, strdup (name));
    to_ret->left = left;
    to_ret->left = right;
    return to_ret;
    }

LangTree *new_lt_empty (LangTree *parent)
    {
    return new_lang_tree (LT_EMPTY, parent);
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

void lang_tree_dump (const LangTree *tree, FILE *f_out, int depth)
    {
    assert (tree);
    assert (f_out);
    
    if (tree->left)
        lang_tree_dump (tree->left, f_out, depth + 4);

    printf ("\n%*s%c", depth, "", tree->type);

    switch (tree->type)
    {
    case LT_ID:
        fprintf (f_out, "%s ", tree->str);
        break;
    
    case LT_NUM:
        fprintf (f_out, "%d ", tree->num);
        break;
    
    case LT_UTIL:
        fprintf (f_out, "%c ", tree->util);
        break;

    case LT_OPER:
        fprintf (f_out, "%c ", tree->oper);
        break;

    case LT_INVALID:
        fprintf (f_out, "invalid type ");    
    case LT_EMPTY:
        fprintf (f_out, "empty ");
        break;

    default:
        fprintf (f_out, "Unknown type ");
        break;
    }
    
    if (tree->right)
        lang_tree_dump (tree->right, f_out, depth + 4);
    }


void lang_tree_graph_node(const LangTree *tree, FILE *gv_out)
    {
    assert(tree);
    assert(gv_out);

    fprintf(gv_out, "\t%lu[shape=record, label=\"", tree);

    switch (tree->type)
    {
    case LT_UTIL:
        switch (tree->util)
        {
        case LTU_STM:
            fprintf (gv_out, "Statement");
            break;
        case LTU_DEF:
            fprintf (gv_out, "Definition");
            break;
        case LTU_FUNC:
            fprintf (gv_out, "Funcction");
            break;
        case LTU_ASSIGN:
            fprintf (gv_out, "=");
            break;
        case LTU_RET:
            fprintf (gv_out, "Return");
            break;
        case LTU_PARAM:
            fprintf (gv_out, "Param");
            break;
        case LTU_IF:
            fprintf (gv_out, "If");
            break;
        case LTU_WHILE:
            fprintf (gv_out, "While");
            break;
        case LTU_DECIS:
            fprintf (gv_out, "Decision");
            break;
        case LTU_CALL:
            fprintf (gv_out, "Call");
            break;
        
        default:
            fprintf (gv_out, "Invalid");
            break;
        }
        break;
    
    case LT_NUM:
        fprintf (gv_out, "%d", tree->num);
        break;
    
    case LT_ID:
        fprintf (gv_out, "%s", tree->str);
        break;

    case LT_OPER:
        if (tree->oper != '>' && tree->oper != '<')
            fprintf (gv_out, "%c", tree->oper);
        else    
            if (tree->oper == '>')
                fprintf (gv_out, "greater");
            else
                fprintf (gv_out, "less");
        break;

    case LT_EMPTY:
        fprintf (gv_out, "null");
        break;

    default:
        fprintf (gv_out, "Unknown\n");
        break;
    }
    
    fprintf (gv_out, "\"];\n");

    if (tree->left) 
        lang_tree_graph_node (tree->left, gv_out);
    if (tree->right) 
        lang_tree_graph_node (tree->right, gv_out);
    }

void lang_tree_graph_arrow(const LangTree *tree, FILE *gv_out)
    {
    assert(tree);
    assert(gv_out);

    if (tree->left)
        fprintf(gv_out, "\t%lu -> %lu[fontsize=12]\n", tree, tree->left);

    if (tree->right)
        fprintf(gv_out, "\t%lu -> %lu[fontsize=12]\n", tree, tree->right);

    if (tree->left)
        lang_tree_graph_arrow (tree->left, gv_out);
    if (tree->right)
        lang_tree_graph_arrow (tree->right, gv_out);
    }

void lang_tree_graph_dump(const LangTree *tree)
    {
    FILE *gv_out = fopen ("lang_tree.gv", "w");

    fprintf (gv_out, "# This is automatically generated dump of language tree\n"
                    "digraph Tree{\n\n"
                    "\trankdir=UD;\n\n"
                    "\tnode[color=\"red\",fontsize=14];\n\n");

    lang_tree_graph_node (tree, gv_out);

    fprintf (gv_out, "\n");

    lang_tree_graph_arrow (tree, gv_out);

    fprintf (gv_out, "\n");

    fprintf (gv_out, "}");

    fclose (gv_out);

    system ("dot -Tpng lang_tree.gv -o lang_tree.png");
    }



