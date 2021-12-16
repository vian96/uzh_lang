#ifndef UZH_FRONTEND_H
#define UZH_FRONTEND_H

#include "../tree/lang_tree.h"
#include "../lexer/lexer.h"

LangTree *lang_frontend (Lexem *code);

LangTree *read_func (Lexem **code, LangTree *parent);

LangTree *read_statement (Lexem **code, LangTree *parent);

LangTree *read_block (Lexem **code, int n, LangTree *parent);

LangTree *read_assign (Lexem **code, LangTree *parent);

LangTree *read_expr (Lexem **code, LangTree *parent);

LangTree *read_return (Lexem **code, LangTree *parent);

LangTree *read_while (Lexem **code, int n, LangTree *parent);

LangTree *read_if (Lexem **code, int n, LangTree *parent);

LangTree *read_var (Lexem **code, LangTree *parent);


#endif // UZH_FRONTEND_H
