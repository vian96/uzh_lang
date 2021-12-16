
typedef int LTNum;

enum LangTreeType {
    LT_INVALID = 0,
    LT_UTIL = 1,
    LT_NUM = 2,
    LT_ID = 3,
    LT_OPER = 4,
};

enum LangTreeOper {
    LTO_INVALID = 0,
    // math op
    LTO_PLUS = '+', LTO_MINUS = '-',
    LTO_MUL = '*', LTO_DIV = '/',
    LTO_POW = '^',
    // compare
    LTO_L = '<', LTO_G = 'g',
    LTO_LEQ = 'm', LTO_GEQ = 'n',
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
};

void lang_tree_lang_ctor (LangTree *tree, LangTree *parent, LangTreeType type, LTNum num = 0, char *str = nullptr, 
                          LangTreeOper oper = LTO_INVALID, LangTreeUtil util = LTU_INVALID, 
                          LangTree *left = nullptr, LangTree *right = nullptr);

void free_tree_lang (LangTree *tree);

LangTree *new_lang_tree (LangTreeType type, LangTree *parent, LTNum num = 0, char *str = nullptr, 
                              LangTreeOper oper = LTO_INVALID, LangTreeUtil util = LTU_INVALID, 
                              LangTree *left = nullptr, LangTree *right = nullptr);

LangTree *new_lt_op (LangTreeOper oper, LangTree *parent, LangTree *left = nullptr, LangTree *right = nullptr);

LangTree *new_lt_util (LangTreeUtil util, LangTree *parent, LangTree *left = nullptr, LangTree *right = nullptr);

LangTree *new_lt_num (LTNum num, LangTree *parent, LangTree *left = nullptr, LangTree *right = nullptr);

LangTree *new_lt_id (char *name, LangTree *parent, LangTree *left = nullptr, LangTree *right = nullptr);

LangTree *read_lang_tree (const char *str, LangTree *parent);

LangTreeOper get_lt_oper (const char **str);

LangTreeUtil get_lt_util (const char **str);

// TODO free of tree
