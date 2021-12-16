// included for size_t
#include <string.h>

typedef int LexNum;

enum LexemType {
    LEX_TERM = 0,
    LEX_NUM = 1,
    LEX_ID = 2,
    LEX_SYM = 3,
    LEX_SPACE = 4,
};

struct Lexem {
    LexemType type;
    LexNum number;
    char *str;
    size_t cnt;
};

Lexem *read_code_lexem (const char *file_name);

Lexem read_lexem (const char **str);

int is_lexem_oper (int sym);

char *copy_str_if (const char **str, int (*cond)(int));

void print_lexems (Lexem *code);

void free_lexems (Lexem *code);