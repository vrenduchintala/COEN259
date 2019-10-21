#ifndef PARSER_H
#define PARSER_H

//match
void match(int token);
static int nextWord();
static void error();

//syntax analysis
void translationUnit();
void globalDeclaration();
void specifier();
void moreDeclarators();
void declarator();
void parameter();
void parameterList();
void declarations();
void declaration();
void statements();
void statement();
void assignment();

//expressions
void expression();
void andExpression();
void equalityExpression();
void relationalExpression();
void additiveExpression();
void multiplicativeExpression();
void unaryExpression();
void primaryExpression();
void argumentList();
void argument();



#endif /* PARSER_H */
