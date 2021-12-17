#ifndef UZH_LANG_TREE_H
#define UZH_LANG_TREE_H

#include <stdio.h>

typedef int LTNum;

enum LangTreeType {
    LT_INVALID = 0,
    LT_UTIL = 'U',
    LT_NUM = 'N',
    LT_ID = 'I',
    LT_OPER = 'O',
    LT_EMPTY = 'E',
};

enum LangTreeOper {
    LTO_INVALID = 0,
    // math op
    LTO_PLUS = '+', LTO_MINUS = '-',
    LTO_MUL = '*', LTO_DIV = '/',
    LTO_POW = '^',
    // compare
    LTO_L = '<', LTO_G = '>',
    LTO_LEQ = 'm', LTO_GEQ = 'b',
    LTO_EQ = 'e', LTO_NEQ = 'n',
    // logic
    LTO_OR = '|', LTO_AND = '&', LTO_NOT = '!',
    // math functions
    LTO_SIN = 's', LTO_COS = 'c', LTO_LN = 'l',
    // UI
    LTO_PRINT = 'p', LTO_SCAN = 'i',
};

enum LangTreeUtil {
    LTU_INVALID = 0,
    LTU_STM = 's',
    LTU_DEF = 'd',
    LTU_FUNC = 'f',
    LTU_ASSIGN = 'a',
    LTU_RET = 'r',
    LTU_PARAM = 'p',
    LTU_IF = 'i',
    LTU_WHILE = 'w',
    LTU_DECIS = 'e',
    LTU_CALL = 'c',
};

struct LangTree {
    LangTreeType type;

    LTNum num;
    char *str;
    LangTreeOper oper;
    LangTreeUtil util;
    
    LangTree *left;
    LangTree *right;
    LangTree *parent;
};

void lang_tree_lang_ctor (LangTree *tree, LangTree *parent = nullptr, LangTreeType type = LT_INVALID, LTNum num = 0, char *str = nullptr, 
                          LangTreeOper oper = LTO_INVALID, LangTreeUtil util = LTU_INVALID, 
                          LangTree *left = nullptr, LangTree *right = nullptr);

void free_tree_lang (LangTree *tree);

LangTree *new_lang_tree (LangTreeType type, LangTree *parent = nullptr, LTNum num = 0, char *str = nullptr, 
                              LangTreeOper oper = LTO_INVALID, LangTreeUtil util = LTU_INVALID, 
                              LangTree *left = nullptr, LangTree *right = nullptr);

LangTree *new_lt_op (LangTreeOper oper, LangTree *parent = nullptr, LangTree *left = nullptr, LangTree *right = nullptr);

LangTree *new_lt_util (LangTreeUtil util, LangTree *parent = nullptr, LangTree *left = nullptr, LangTree *right = nullptr);

LangTree *new_lt_num (LTNum num, LangTree *parent = nullptr, LangTree *left = nullptr, LangTree *right = nullptr);

LangTree *new_lt_id (char *name, LangTree *parent = nullptr, LangTree *left = nullptr, LangTree *right = nullptr);

LangTree *new_lt_empty (LangTree *parent = nullptr);

LangTree *read_lang_tree (const char *str, LangTree *parent = nullptr);

LangTreeOper get_lt_oper (const char **str);

LangTreeUtil get_lt_util (const char **str);

void lang_tree_dump (const LangTree *tree, FILE *f_out, int depth = 0);

void lang_tree_graph_node(const LangTree *tree, FILE *gv_out);

void lang_tree_graph_arrow(const LangTree *tree, FILE *gv_out);

void lang_tree_graph_dump(const LangTree *tree);


#endif // UZH_LANG_TREE_H
