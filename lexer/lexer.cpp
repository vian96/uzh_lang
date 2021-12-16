#include "lexer.h"

#include <onegin/file_utils.h>
#include <asm/asm/string_utils.h>

#include <ctype.h>
#include <string.h>

// #define DEB(...) printf (__VA_ARGS__)
#define DEB(...) 

LexemFile read_code_lexem (const char *file_name)
    {
    assert (file_name);

    FILE *f_in = fopen_err (file_name, "r");
    FileText code = read_text_file (f_in);
    fclose (f_in);
    f_in = nullptr;

    DEB ("Opened file\n");

    LexemFile to_ret = {};
    to_ret.lexs = (Lexem*) calloc (code.count * 100, sizeof (*to_ret.lexs));

    DEB ("Allocated lexems\n");

    for (size_t line = 0; line < code.count; line++) 
        {
        char *str = code.strings[line].begin;
        
        while (*str) 
            {
            // removes comments
            if (*str == '#') 
                {
                *str = '\0';
                break;
                }
            str++;
            }
        }

    DEB ("Removed comments\n");

    size_t cur_pos = 0;
    for (size_t line = 0; line < code.count; line++) 
        {
        const char *str = code.strings[line].begin;

        if (is_empty_string (str))
            continue;

        size_t cnt_space = 0;
        while (*str == ' ')
            {
            cnt_space++;
            str++;
            }
        to_ret.lexs[cur_pos].type = LEX_SPACE;
        to_ret.lexs[cur_pos].cnt = cnt_space;
        cur_pos++;

        DEB ("Begin to analyze str %s\n", str);

        while (*str)
            {
            to_ret.lexs[cur_pos] = read_lexem (&str);
            cur_pos++;
            skip_blank (&str);
            }
        }

    DEB ("Read all lexems\n");
    to_ret.cnt = cur_pos; // +1 is not needed because cur_pos always point to next pos
    free_file_text (&code);
    DEB ("Freed file\n");
    
    return to_ret;
    }

Lexem read_lexem (const char **str)
    {
    assert (str);
    assert (*str);

    Lexem to_ret = {};

    if (isalpha (**str))
        {
        DEB ("Reading id %s\n", *str);
        to_ret.type = LEX_ID;
        to_ret.str = copy_str_if (str, isalnum);
        return to_ret;
        }

    if (isdigit (**str))
        {
        DEB ("Reading number %s\n", *str);
        // TODO if change to double change code
        LexNum num = 0;
        if (sscanf (*str, "%d", &num))
            {
            to_ret.type = LEX_NUM;
            to_ret.number = num;
            skip_alnum (str);
            return to_ret;
            }
        printf ("Syntax ERROR: id begins with number but it is not number at line %s\n", *str);
        return {};
        }
    
    DEB ("Reading sym %s\n", *str);
    to_ret.type = LEX_SYM;
    to_ret.str = copy_str_if (str, is_lexem_oper);
    return to_ret;
    }

int is_lexem_oper (int sym)
    {
    return sym && (bool) strchr ("+-*/^<>=!|&():", sym);
    }

char *copy_str_if (const char **str, int (*cond)(int))
    {
    assert (str);
    assert (*str);
    assert (cond);

    size_t word_len = 0;

    const char *tmp = *str;
    while (cond (*tmp))
        {
        word_len++;
        tmp++;
        }

    char *to_ret = (char*) calloc (word_len + 1, sizeof (*to_ret)); // +1 because of '\0'
    char *ptr = to_ret;
    while (cond (**str))
        {
        *ptr = **str;
        (*str)++;
        ptr++;
        }

    return to_ret;
    }

void print_lexems (LexemFile code)
    {
    for (int i = 0; i < code.cnt; i++)
        {
        Lexem *temp = code.lexs + i;
        switch (temp->type)
        {
        case LEX_ID:
        case LEX_SYM:
            printf ("%s ", temp->str);
            break;

        case LEX_NUM:
            printf ("%d ", temp->number);
            break;

        case LEX_SPACE:
            printf ("\n%*s", temp->cnt, "");
            break;

        default:
            printf ("ERROR: unknown type of lex\n");
            break;
        }
        }
    }

void free_lexems (LexemFile code)
    {
    for (size_t i = 0; i < code.cnt; i++)
        free (code.lexs[i].str);
    free (code.lexs);
    }

