#ifndef UZH_LEXER_H
#define UZH_LEXER_H

// included for size_t
#include <string.h>

typedef int LexNum;

enum LexemType {
    LEX_TERM = 0,
    LEX_NUM = 'N',
    LEX_ID = 'I',
    LEX_SYM = 'S',
    LEX_SPACE = '_',
};

struct Lexem {
    LexemType type;
    LexNum number;
    char *str;
    size_t cnt;
};

Lexem *read_code_lexem (const char *file_name);

Lexem read_lexem (const char **str);

char *copy_str_if (const char **str, int (*cond)(int));

void print_lexems (Lexem *code);

void print_lexem_line (Lexem *code);

void print_lexem (Lexem lex);

void free_lexems (Lexem *code);

#endif // UZH_LEXER_H
