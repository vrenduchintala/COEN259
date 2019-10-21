#include "parser.h"
#include "tokens.h"
#include "lexer.h"

#include <string>
#include <iostream>
#include <stdio.h>

using namespace std;

int word;
string lexeme;

/*
 * Function: specifier
 *
 * Description: check to see if we can get next floor
 */
void specifier()
{
	if(word == INT || word == CHAR)
  {
		word = nextWord();
  }
	else
  {
		error();
  }
}
/*
 * Function: match
 *
 * Description: match given token against current token
 * 
 * Parameters: 
 * 	int token: token to be matched
 */
void match(int token)
{
	if(word == token)
		word = nextWord();
	else
  {
		error();
  }
}

/*
 * Function: parameter
 *
 * Description: match a parameter
 */
void parameter() 
{
	specifier();
	match(NAME);
	if(word == '[')
	{
		match('[');
		match(']');
	}
}

/*
 * Function: parameterList
 * 
 * Description: match a list of parameters
 */
void parameterList()
{  
  parameter();
  
  while(word == ',')
  {
    match(',');
    parameter();
  }
}

/*
 * Function: parameters
 *
 * Description: get the parameters
 */
void parameters()
{
  if(word == VOID)
  {
    match(VOID);
  }
  else
  {
    parameterList();
  }
}

/*
 * Function: declarator
 *
 * Description: match a declarator
 */
void declarator() 
{
	match(NAME);

	if(word == '[')
	{
		match('[');
    match(NUM);
		match(']');
	}
}


/*
 * Function: moreDeclarators
 * 
 * Description: match a list of declarators
 */
void moreDeclarators()
{
  while(word == ',')
  {
    match(',');
    declarator();
  }
}

/*
 * Function: declaration
 *
 * Description: get declaration
 */
void declaration()
{
  specifier();
  declarator();
  moreDeclarators();
  match(';');
}

/*
 * Function: declarations
 *
 * Description: get list of declarations
 */
void declarations()
{
  while (word == INT || word == CHAR)
    declaration();
}

/*
 * Function: assignment
 *
 * Description: get assignment
 */
void assignment()
{
  match(NAME);
  if(word == '=')
  {
    match('=');
    expression();
  }
  else if(word == '[')
  {
    match('[');
    expression();
    match(']');
    match('=');
    expression();
  }
  else if(word == '(')
  {
    match('(');
    if(word == ')')
    {
      match(')');
      return;
    }
    argumentList();
    match(')');
  }
}

/*
 * Function: statement
 *
 * Description: Get the statement
 */
void statement()
{
  if(word == '{')
  {
    match('{');
    statements();
    match('}');
  }
  else if(word == IF)
  {
    match(IF);
    match('(');
    expression();
    match(')');
    statement();
    if(word == ELSE)
    {
      match(ELSE);
      statement();
    }
  }
  else if(word == FOR)
  {
    match(FOR);
    match('(');
    assignment();
    match(';');
    expression();
    match(';');
    assignment();
    match(')');
    statement();
  }
  else if(word == WHILE)
  {
    match(WHILE);
    match('(');
    expression();
    match(')');
    statement();
  }
  else if(word == DO)
  {
    match(DO);
    statement();
    match(WHILE);
    match('(');
    expression();
    match(')');
    match(';');
  }
  else if(word == RETURN)
  {
    match(RETURN);
    expression();
    match(';');
  }
  else
  {
    assignment();
    match(';');
  }
}

/*
 * Function: statements
 *
 * Description: get a list of statements
 */
void statements() 
{
  while (word != '}')
    statement();
}

/*
 * Function: globalDeclaration
 *
 * Definition: get the global declarations
 */
void globalDeclaration()
{
  specifier();
  match(NAME);
  if(word == '[')
  {
    match('[');
    match(NUM);
    match(']');
    moreDeclarators();
    match(';');
  }
  else if(word == '(')
  {
    match('(');
    parameters();
    match(')');
    match('{');
    declarations();
    statements();
    match('}');
  }
  else
  {
    moreDeclarators();
    match(';');
  }
}

/*
 * Function: translationUnit
 *
 * Definition: start translating
 */
void translationUnit()
{
   while (word == INT || word == CHAR)
    globalDeclaration();
}

/*
 * Function: expression
 *
 * Definition: get expression
 */
void expression()
{
  andExpression();
  while(word == OR)
  {
    match(OR);
    andExpression();
  }
}

/*
 * Function: andExpression
 *
 * Definiton: get and expression
 */
void andExpression()
{
  equalityExpression();
  while(word == AND)
  {
    match(AND);
    equalityExpression();
  }
}

/*
 * Function: equalityExpression
 *
 * Definiton: get equality expression
 */
void equalityExpression()
{
  relationalExpression();
  while(word == EQL || word == NEQ)
  {
    if(word == EQL)
    {
      match(EQL);
    }
    else
    {
      match(NEQ);
    }
    relationalExpression();
  }
}

/*
 * Function: relationalExpression
 *
 * Definiton: get relational expression
 */
void relationalExpression()
{
  additiveExpression();
  while(word == '<' || word == '>' || word == LEQ || word == GEQ)
  {
    if(word == '<')
    {
      match('<');
    }
    else if(word == '>')
    {
      match('>');
    }
    else if(word == LEQ)
    {
      match(LEQ);
    }
    else
    {
      match(GEQ);
    }
    additiveExpression();
  }
}

/*
 * Function: additiveExpression
 *
 * Definiton: get additive expression
 */
void additiveExpression()
{
  multiplicativeExpression();
  while(word == '+' || word == '-')
  {
    if(word == '+')
    {
      match('+');
    }
    else
    {
      match('-');
    }
    multiplicativeExpression();
  }
}

/*
 * Function: multiplicativeExpression
 *
 * Definiton: get multiplicative expression
 */
void multiplicativeExpression()
{
  unaryExpression();
  while(word == '*' || word == '/' || word == '%')
  {
    if(word == '*')
    {
      match('*');
    }
    else if(word == '/')
    {
      match('/');
    }
    else
    {
      match('%');
    }
    unaryExpression();
  }
}

/*
 * Function: unaryExpression
 *
 * Definition: get unary expression
 */
void unaryExpression()
{
  if(word == '-')
  {
    match('-');
    unaryExpression();
  }
  else if(word == '!')
  {
    match('!');
    unaryExpression();
  }
  else
  {
    primaryExpression();
  }
}

/*
 * Function: primaryExpression
 *
 * Definition: get primary expression
 */
void primaryExpression()
{
  if(word == '(')
  {
    match('(');
    expression();
    match(')');
  }
  else if(word == CHARLIT)
  {
    match(CHARLIT);
  }
  else if(word == NUM)
  {
    match(NUM);
  }
  else
  {
    match(NAME);
    if(word == '[')
    {
      match('[');
      expression();
      match(']');
    }
    else if(word == '(')
    {
      match('(');
      if(word != ')')
      {
        argumentList();
      }
      match(')');
    }
  }
}

/*
 * Function: argument
 *
 * Definition: get argument
 */
void argument()
{
  if(word == STRLIT)
  {
    match(STRLIT);
  }
  else
  {
    expression();
  }
}


/*
 * Function: argumentList
 *
 * Definiton: get argument list
 */
void argumentList()
{
  argument();
  
  while(word == ',')
  {
    match(',');
    argument();
  }
}

/*
 * Function: nextWord
 *
 * Description: Get the next token in the sequence
 */ 
static int nextWord()
{
	int token = yylex();
	lexeme = yytext;
	return token;
}


/*
 * Function: error
 *
 * Description: Report a syntax error to standard error.
 */
static void error()
{
  cerr << "line " << yylineno;
  cerr << ": syntax error at '" << yytext << "'" << endl;
	exit(EXIT_FAILURE);
}

int main()
{
  word = nextWord();
  translationUnit();
}
