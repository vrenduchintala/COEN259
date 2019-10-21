%{
# include <iostream>
# include "tokens.h"
using namespace std;
%}

%option nounput noyywrap yylineno

%%

auto                        {return AUTO;}
break                       {return BREAK;}
case                        {return CASE;}
char                        {return CHAR;}
const                       {return CONST;}
continue                    {return CONTINUE;}
default                     {return DEFAULT;}
do                          {return DO;}
double                      {return DEFAULT;}
else                        {return ELSE;}
enum                        {return ENUM;}
extern                      {return EXTERN;}
float                       {return FLOAT;}
for                         {return FOR;}
goto                        {return GOTO;}
if                          {return IF;}
int                         {return INT;}
long                        {return LONG;}
register                    {return REGISTER;}
return                      {return RETURN;}
short                       {return SHORT;}
signed                      {return SIGNED;}
sizeof                      {return SIZEOF;}
static                      {return STATIC;}
struct                      {return STRUCT;}
switch                      {return SWITCH;}
typedef                     {return TYPEDEF;}
union                       {return UNION;}
unsigned                    {return UNSIGNED;}
void                        {return VOID;}
volatile                    {return VOLATILE;}
while                       {return WHILE;}

"="                         {return ASSIGN;}
"||"                        {return OR;}
"&&"                        {return AND;}
"=="                        {return EQL;}
"!="                        {return NEQ;}
"<"                         {return LTN;}
">"                         {return GTN;}
"<="                        {return LEQ;}
">="                        {return GEQ;}
"+"                         {return PLUS;}
"-"                         {return MINUS;}
"*"                         {return STAR;}
"/"                         {return DIV;}
"%"                         {return REM;}
"!"                         {return NOT;}
"++"                        {return INC;}
"--"                        {return DEC;}
"("                         {return LPAREN;}
")"                         {return RPAREN;}
"["                         {return LBRACK;}
"]"                         {return RBRACK;}
"{"                         {return LBRACE;}
"}"                         {return RBRACE;}
";"                         {return SEMI;}
","                         {return COMMA;}

[_a-zA-Z][_a-zA-Z0-9]*      {return NAME;}
[0-9]+                      {return NUM;}

\"(\\[^\n]|[^\\\n\"])*\"    {return STRLIT;}
'(\\[^\n]|[^\\\n'])*'       {return CHARLIT;}

(\ |\n|\t|\r|\f|\v)+ 		    {/*ignored*/}
\/\*([^*]|\*+[^/])*\*+\/		{/*ignored*/}

.                           {return ERROR;}
%%
