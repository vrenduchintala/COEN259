/*
 * File:	parser.c
 *
 * Description:	This file contains the public and private function and
 *		variable definitions for the recursive-descent parser for
 *		Tiny C.
 */

# include <string>
# include <cstdlib>
# include <iostream>
# include <vector>
# include <map>
# include "lexer.h"
# include "tokens.h"
# include "type.h"

#define SCALAR 0
#define ARRAY 1
#define FUNCTION 2
#define LITERAL 3

using namespace std;

static int word;
static string lexeme;
static int peeked;
static void expression(), statement();

//Global Scope
static map<string, type> globals;

//Local Scope
static map<string, type> locals;

//Current Scope
static map<string, type>* curr = &globals;

//Functions
static map<string, vector<type>> functions;

/*
 * Function: findInScope
 *
 * Description: Find a variable in scope
 *
 * Parameters: int scope = which scope to check
 *             string name = name to check
 */
/*
static int findInScope(int scope, string name)
{
  vector<vector<string>> currScope;
  if(scope == 1)
  {
    if(globals.count(name) > 0)
    {
      return 1;
    }
  }
  else
  {
     if(globals.count(name) > 0)
    {
      return 1;
    }
  }
  return 0;
}
*/
/*
 * Function: Report
 *
 * Description: Report error to stderr
 *
 * Parameters: string name = name of variable
 *             int error = error code for message
 */
static void report(string name, int error)
{
  string message = "";

  if(error == 1)
  {
    message = message +  "'" + name + "' redeclared";
  }
  else if(error == 2)
  {
    message = message +  "'" + name + "' undeclared";
  }
  else if(error == 3)
  {
    message = message + "array type required for '" + name + "'";
  }
  else if(error == 4)
  {
     message = message + "function type required for '" + name + "'";
  }
  else if(error == 5)
  {
     message = message + "invalid arguments to '" + name + "'";
  }
  else
  {
     message = message + "scalar type required for '" + name + "'";
  }

  cerr << "Line No: "<< yylineno << " " << message << endl;
}


/*
 * Function: peek
 *
 * Description: peek the next word of the lexer
 */
static int peek()
{
  if(peeked == 0)
    peeked = yylex();

  return peeked;
}
/*
 * Function:	nextWord
 *
 * Description:	Return the next word from the lexer.  The textbook calls
 *		such a function 'nextWord' and calls the text that was
 *		matched 'lexeme' so we do as well.
 */

static int nextWord()
{
    int token;
    
    if (peeked != 0)
    {
      token = peeked;
      peeked = 0;
    }
    else
    {
      token = yylex();
    }
    lexeme = yytext;
    return token;
}


/*
 * Function:	error
 *
 * Description:	Report a syntax error to standard error.
 */

static void error()
{
    cerr << "line " << yylineno;
    cerr << ": syntax error at '" << yytext << "'" << endl;
    exit(EXIT_FAILURE);
}


/*
 * Function:	match
 *
 * Description:	Match the next token against the specified token.  A
 *		failure indicates a syntax error and will terminate the
 *		program since our parser does not do error recovery.
 */

static void match(int token)
{
    if (word != token)
	error();

    word = nextWord();
}


/*
 * Function:	argument
 *
 * Description:	Parse an argument to a function call.  The only place
 *		string literals are allowed in Tiny C is here, to enable
 *		calling printf(), scanf(), and the like.
 *
 *		Argument:
 *		  strlit
 *		  Expression
 */

static int argument(int counter, string func, int dec)
{
  string name = lexeme;
  if (word == STRLIT)
  {
    match(STRLIT);
    return 0;
  }
  else if(word == NAME && (peek() == ',' || peek() == ')'))
  {
    type t;
    if(locals.count(name) != 0)
    {

      t = locals[name];
    }
    else if(globals.count(name) != 0)
    {
      t = globals[name];
    }
    else
    {
      report(name, 2);
    }
    if(dec == 0)
    {
      if(counter >= functions[func].size())
      {
        report(func, 5);
      }
      else
      {
        type comp = functions[func][counter];
        //cout <<name<<": "<<t.specifier<<","<<t.declarator<<"   "<<func<<": "<<comp.specifier<<", "<<comp.declarator<<endl;
        if(comp.declarator != t.declarator)
        {
          report(func, 5);
        }
        else
        {
          if(comp.declarator == ARRAY)
          {
            if(comp.specifier != t.specifier)
              report(func, 5);
          }
        }
      }
    }
    else
    {
      functions[func].push_back(t);
    }
    match(NAME);
  }
  else
	  expression();
  return 1;
}


/*
 * Function:	argumentList
 *
 * Description:	Parse the argument list of a function call.
 *
 *		ArgumentList:
 *		  Argument
 *		  Argument , ArgumentList
 */

static void argumentList(string func, int dec)
{
  int counter = 0;
  if(argument(counter, func, dec) == 1)
  {
    counter++;
  }

  while (word == ',') {
	  match(',');
	  if(argument(counter, func, dec) == 1)
    {
      counter++;
    }
  }
}


/*
 * Function:	primaryExpression
 *
 * Description:	Parse a primary expression, which will always have a
 *		scalar type.
 *
 *		PrimaryExpression:
 *		  ( Expression )
 *		  charlit
 *		  num
 *		  name
 *		  name ( )
 *		  name ( ExpressionList )
 *		  name [ Expression ]
 */

static void primaryExpression()
{
  if (word == '(') {
	  match('(');
	  expression();
	  match(')');
  } 
  else if (word == CHARLIT) {
	  match(CHARLIT);

  }
  else if (word == NUM) {
	  match(NUM);

  }
  else {
    string name = lexeme;
    match(NAME);
    
    if(curr->count(name) == 0 && globals.count(name) == 0 && word != '(')
    {
      report(name, 2);
    }
    
    if (word == '(') {
      int dec = 0;
      if(curr->count(name) == 0 && globals.count(name) == 0)
      {
        type t;
        t.specifier = 0;
        t.declarator = FUNCTION;
        globals.insert({name, t});
        dec = 1;
      }
      if(curr->count(name) != 0)
      {
        if((*curr)[name].declarator != FUNCTION)
        {
          report(name, 4);
        }
      }
      else if(globals.count(name) != 0)
      {
        if(globals[name].declarator != FUNCTION)
        {
          report(name, 4);
        }
      }

      /*
      if((*curr)[name].declarator != FUNCTION && globals[name].declarator != FUNCTION)
      {
        report(name, 4);
      }
      */

      match('(');
	    if (word != ')')
	    	argumentList(name, dec);
	    match(')');

	  }
    else if (word == '[') {
      if(curr->count(name) != 0)
      {
        if((*curr)[name].declarator != ARRAY)
        {
          report(name, 3);
        }
      }
      else if(globals.count(name) != 0)
      {
        if(globals[name].declarator != ARRAY)
        {
          report(name, 3);
        }
      }


      /*
      if((*curr)[name].declarator != ARRAY && globals[name].declarator != ARRAY)
      {
        report(name, 4);
      }
      */
	    match('[');
	    expression();
	    match(']');

	  }
    else{

      if(curr->count(name) != 0)
      {
        if((*curr)[name].declarator != SCALAR)
        {
          report(name, 6);
        }
      }
      else if(globals.count(name) != 0)
      {
        if(globals[name].declarator != SCALAR)
        {
          report(name, 6);
        }
      }

      /*
      if((*curr)[name].declarator != SCALAR && globals[name].declarator != SCALAR)
      {
        report(name, 6);
      }
      */
    }
  }
}


/*
 * Function:	unaryExpression
 *
 * Description:	Parse a unary expression.
 *
 *		UnaryExpression:
 *		  - UnaryExpression
 *		  ! UnaryExpression
 *		  PrimaryExpression
 */

static void unaryExpression()
{
  if (word == '-') {
    match('-');
    unaryExpression();

  } 
  else if (word == '!') {
    match('!');
    unaryExpression();

  } 
  else
    primaryExpression();
}


/*
 * Function:	multiplicativeExpression
 *
 * Description:	Parse a multiplicative expression.
 *
 *		MultiplicativeExpression:
 *		  UnaryExpression
 *		  MultiplicativeExpression * UnaryExpression
 *		  MultiplicativeExpression / UnaryExpression
 *		  MultiplicativeExpression % UnaryExpression
 */

static void multiplicativeExpression()
{
  unaryExpression();

  while (1) {
    if (word == '*') {
        match('*');
        unaryExpression();

    } 
    else if (word == '/') {
        match('/');
        unaryExpression();

    } 
    else if (word == '%') {
        match('%');
        unaryExpression();

    } 
    else
        break;
  }
}


/*
 * Function:	additiveExpression
 *
 * Description:	Parse an additive expression.
 *
 *		AdditiveExpression:
 *		  MultiplicativeExpression
 *		  AdditiveExpression + MultiplicativeExpression
 *		  AdditiveExpression - MultiplicativeExpression
 */

static void additiveExpression()
{
    multiplicativeExpression();

    while (1) {
	if (word == '+') {
	    match('+');
	    multiplicativeExpression();

	} else if (word == '-') {
	    match('-');
	    multiplicativeExpression();

	} else
	    break;
    }
}


/*
 * Function:	relationalExpression
 *
 * Description:	Parse a relational expression.
 *
 *		RelationalExpression:
 *		  AdditiveExpression
 *		  RelationalExpression < AdditiveExpression
 *		  RelationalExpression > AdditiveExpression
 *		  RelationalExpression <= AdditiveExpression
 *		  RelationalExpression >= AdditiveExpression
 */

static void relationalExpression()
{
    additiveExpression();

    while (1) {
	if (word == '<') {
	    match('<');
	    additiveExpression();

	} else if (word == '>') {
	    match('>');
	    additiveExpression();

	} else if (word == LEQ) {
	    match(LEQ);
	    additiveExpression();

	} else if (word == GEQ) {
	    match(GEQ);
	    additiveExpression();

	} else
	    break;
    }
}


/*
 * Function:	equalityExpression
 *
 * Description:	Parse an equality expression.
 *
 *		EqualityExpression:
 *		  RelationalExpression
 *		  EqualityExpression == RelationalExpression
 *		  EqualityExpression != RelationalExpression
 */

static void equalityExpression()
{
    relationalExpression();

    while (1) {
	if (word == EQL) {
	    match(EQL);
	    relationalExpression();

	} else if (word == NEQ) {
	    match(NEQ);
	    relationalExpression();

	} else
	    break;
    }
}


/*
 * Function:	andExpression
 *
 * Description:	Parse a logical-and expression.
 *
 *		AndExpression:
 *		  EqualityExpression
 *		  AndExpression && EqualityExpression
 */

static void andExpression()
{
    equalityExpression();

    while (word == AND) {
	match(AND);
	equalityExpression();
    }
}


/*
 * Function:	expression
 *
 * Description:	Parse an expression, or more specifically a logical-or
 *		expression, since Tiny C does not allow comma or
 *		assignment as an expression operator.
 *
 *		Expression:
 *		  AndExpression
 *		  Expression || AndExpression
 */

static void expression()
{
    andExpression();

    while (word == OR) {
	match(OR);
	andExpression();
    }
}


/*
 * Function:	assignment
 *
 * Description:	Parse an assignment statement, or more specifically a
 *		simple expression statement that has a side effect,
 *		which includes assignments and function calls.
 *
 *		Assignment:
 *		  name = Expression
 *		  name [ Expression ] = Expression
 *		  name ( ExpressionList )
 *		  name ( )
 */

static void assignment()
{
    string name = lexeme;
    match(NAME);

    if (word == '=') {
      if(curr->count(name) == 0 && globals.count(name) == 0)
      {
        report(name, 2);
      }

      if(curr->count(name) != 0)
      {
        if((*curr)[name].declarator != SCALAR)
        {
          report(name, 6);
        }
      }
      else if(globals.count(name) != 0)
      {
        if(globals[name].declarator != SCALAR)
        {
          report(name, 6);
        }
      }

      /*
      if((*curr)[name].declarator != SCALAR && globals[name].declarator != SCALAR)
      {
        report(name, 6);
      }
      */
	    match('=');
	    expression();

    } 
    else if (word == '[') {
      if(curr->count(name) == 0 && globals.count(name) == 0)
      {
        report(name, 2);
      }
      //cout << name << endl;
      if(curr->count(name) != 0)
      {
        if((*curr)[name].declarator != ARRAY)
        {
          report(name, 3);
        }
      }
      else if(globals.count(name) != 0)
      {
        if(globals[name].declarator != ARRAY)
        {
          report(name, 3);
        }
      }

      /*
      if((*curr)[name].declarator != ARRAY && globals[name].declarator != ARRAY)
      {
        report(name, 3);
      }
      */
	    match('[');
	    expression();
	    match(']');
	    match('=');
	    expression();

    } 
    else if (word == '(') {
      int dec = 0;
      if(curr->count(name) == 0 && globals.count(name) == 0)
      {
        type t;
        t.specifier = 0;
        t.declarator = FUNCTION;
        globals.insert({name, t});
        dec = 1;
      }

      if(curr->count(name) != 0)
      {
        if((*curr)[name].declarator != FUNCTION)
        {
          report(name, 4);
        }
      }
      else if(globals.count(name) != 0)
      {
        if(globals[name].declarator != FUNCTION)
        {
          report(name, 4);
        }
      }

      /*
      if((*curr)[name].declarator != FUNCTION && globals[name].declarator != FUNCTION)
      {
        report(name, 6);
      }
      */
	    match('(');

	    if (word != ')')
	        argumentList(name, dec);

	    match(')');

    } 
    else
	    error();
}


/*
 * Function:	statements
 *
 * Description:	Parse a possibly empty sequence of statements.  Rather than
 *		checking if the next token starts a statement, we check if
 *		the next token ends the sequence, since a sequence of
 *		statements is always terminated by a closing brace.
 *
 *		Statements:
 *		  empty
 *		  Statement Statements
 */

static void statements()
{
    while (word != '}')
	    statement();
}


/*
 * Function:	statement
 *
 * Description:	Parse a statement.  Note that Tiny C has so few
 *		statements that we handle them all in this one function.
 *
 *		Statement:
 *		  { Statements }
 *		  if ( Expression ) Statement
 *		  if ( Expression ) Statement else Statement
 *		  for ( Assignment ; Expression ; Assignment ) Statement
 *		  while ( Expression ) Statement
 *		  do Statement while ( Expression ) ;
 *		  return Expression ;
 *		  Assignment ;
 */

static void statement()
{
  if (word == '{') {
    match('{');
    statements();
    match('}');

  } 
  else if (word == IF) {
    match(IF);
    match('(');
    expression();
    match(')');
    statement();

    if (word == ELSE) {
      match(ELSE);
      statement();
    }

  } 
  else if (word == FOR) {
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
  else if (word == WHILE) {
    match(WHILE);
    match('(');
    expression();
    match(')');
    statement();

  } 
  else if (word == DO) {
    match(DO);
    statement();
    match(WHILE);
    match('(');
    expression();
    match(')');
    match(';');

  } 
  else if (word == RETURN) {
    match(RETURN);
    expression();
    match(';');

  } 
  else {
    assignment();
    match(';');
  }
}


/*
 * Function:	specifier
 *
 * Description:	Parse a type specifier, which in Tiny C is char or int.
 *
 * Return: 0 for int, 1 for char
 */

static int specifier()
{
    if (word == INT)
    {
	    match(INT);
      return 0;
    }
    else
    {
	    match(CHAR);
      return 1;
    }
    return 0;
}


/*
 * Function:	parameter
 *
 * Description:	Parse a parameter, which in Tiny C is either a scalar
 *		or an array in which the size is not specified.
 *
 *		Parameter:
 *		  Specifier name
 *		  Specifier name [ ]
 */

static void parameter(string func)
{
    int spec = specifier();
    string name  = lexeme;
    match(NAME);
    
    type t;
    t.specifier = spec;
    if (word == '[') {
      t.declarator = 1;
	    match('[');
	    match(']');
    }
    else
    {
      t.declarator = 0;
    }

    if(locals.count(name) == 0)
    {
      locals.insert({name, t});
    }
    else
    {
      report(name, 1);
    }
    functions[func].push_back(t);
    //cout << name<< " size: " << functions[name].size() << endl;
}


/*
 * Function:	parameterList
 *
 * Description:	Parse a non-empty parameter list.
 *
 *		ParameterList:
 *		   Parameter
 *		   Parameter , ParameterList
 */

static void parameterList(string func)
{
    parameter(func);

    while (word == ',') {
	match(',');
	parameter(func);
    }
}


/*
 * Function:	parameters
 *
 * Description:	Parse the parameters of a function, but not the
 *		opening or closing parentheses.
 *
 *		Parameters:
 *		  void
 *		  ParameterList
 */

static void parameters(string func)
{
    if (word == VOID)
	match(VOID);
    else
	parameterList(func);
}


/*
 * Function:	declarator
 *
 * Description:	Parse a declarator, which in Tiny C is either a scalar
 *		or a single-dimensional array.
 *
 *		Declarator:
 *		  name
 *		  name [ num ]
 */

static void declarator(int spec)
{
  string name = lexeme;
  match(NAME);

  type t;
  t.specifier = spec;
  /*
  if(curr->count(name) != 0)
    report(name, 1);
  */
  if (word == '[') {
    t.declarator = ARRAY;
	  match('[');
	  match(NUM);
	  match(']');
  }
  else
  {
    t.declarator = SCALAR;
  }
  
  if(curr->count(name) != 0)
  {
    report(name, 1);
  }
  else
  {
    curr->insert({name, t});
  }
}


/*
 * Function:	moreDeclarators
 *
 * Description:	Parse any remaining declarators in a declaration after
 *		the first declarator.
 *
 *		MoreDeclarators:
 *		  empty
 *		  , Declararor MoreDeclarators
 */

static void moreDeclarators(int spec)
{
  while (word == ',') {
	  match(',');
	  declarator(spec);
  }
}


/*
 * Function:	declaration
 *
 * Description:	Parse a single declaration.
 *
 *		Declaration:
 *		  Specifier Declarator MoreDeclarators ;
 */

static void declaration()
{
  int spec = specifier();
  declarator(spec);
  moreDeclarators(spec);
  match(';');
}


/*
 * Function:	declarations
 *
 * Description:	Parse a possible empty sequence of declarations.
 *
 *		Declarations:
 *		  empty
 *		  Declaration Declarations
 */

static void declarations()
{
    while (word == CHAR || word == INT)
	declaration();
}


/*
 * Function:	globalDeclaration
 *
 * Description:	Parse a global (i.e., top-level) declaration, which is
 *		either a variable declaration or a function definition.
 *
 *		GlobalDeclaration:
 *		  Specifier name MoreDeclarators ;
 *		  Specifier name [ num ] MoreDeclarators ;
 *		  Specifier name ( Parameters ) { Declarations Statements }
 */

static void globalDeclaration()
{
  int spec = specifier();
  string name = lexeme;
  match(NAME);
  

  type t;
  t.specifier = spec;

  if (word == '[') {
    t.declarator = ARRAY;
    if(globals.count(name) != 0)
    {
      report(name, 1);
    }
    else
    {
      globals.insert({name, t});
    }
	  match('[');
    match(NUM);
	  match(']');
    moreDeclarators(spec);
    match(';');
  } 
  else if (word == '(') {
    t.declarator = FUNCTION;
    if(globals.count(name) != 0)
    {
      report(name, 1);
    }
    else
    {
      globals.insert({name, t});
    }

	  match('(');

    curr = &locals;
    /*
    cout <<"locals:"  << endl;
    
    for(auto it = curr->cbegin(); it != curr->cend(); ++it)
    {
      cout << endl;
      cout <<"locals:"  << endl;
      std::cout << it->first << " " << it->second.specifier << " " << it->second.declarator << "\n";
    }
    */
    functions[name];
	  parameters(name);
	  match(')');
	  match('{');
	  declarations();
	  statements();
	  match('}');

    /*
    cout <<"locals:"  << endl;
    for(auto it = curr->cbegin(); it != curr->cend(); ++it)
    {
      std::cout << it->first << " " << it->second.specifier << " " << it->second.declarator << "\n";
    }
    cout << endl;
    */
    
    curr = &globals;

    
    /*
    cout <<"functions:"  << endl;
    
    for(auto it = functions.cbegin(); it != functions.cend(); ++it)
    {
      std::cout << it->first << "\n";
    }
    */
    


    locals.clear();
  } 
  else 
  {
    t.declarator = SCALAR;
    if(globals.count(name) != 0)
    {
      report(name, 1);
    }
    else
    {
      globals.insert({name, t});
    }
    moreDeclarators(spec);
    match(';');
  }

  /*
  cout << endl;
  cout <<"globals:"  << endl;
  for(auto it = globals.cbegin(); it != globals.cend(); ++it)
  {
    std::cout << it->first << " " << it->second.specifier << " " << it->second.declarator << "\n";
  }
  */
  
}


/*
 * Function:	translationUnit
 *
 * Description:	Parse the current translation unit (i.e., file), which
 *		consists of a possibly empty sequence of global
 *		declarations.
 *
 *		TranslationUnit:
 *		  empty
 *		  GlobalDeclaration TranslationUnit
 */

static void translationUnit()
{
    while (word != DONE)
	globalDeclaration();
}


/*
 * Function:	main
 *
 * Description:	Analyze the standard input stream.
 */

int main()
{
    word = nextWord();
    translationUnit();
}
