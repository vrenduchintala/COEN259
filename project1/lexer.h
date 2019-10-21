#ifndef LEXER_H
#define LEXER_H
#include <string>

extern char *yytext;
extern int yylineno, numerrors;

int yylex();
void report(const std::string &str, const std::string &arg = "");

#endif /* LEXER_H */
