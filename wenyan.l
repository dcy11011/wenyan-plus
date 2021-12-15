%{
#include <iostream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <string>

#include "ErrorLog.h"
#include "AST.h"
#include "wenyan.x.tab.h"
extern YYSTYPE yylval;

int n_line = 1;
int n_char = 1;

TextPosition readtoken(char* str){
   TextPosition ret(n_line, n_char);
   n_char += strlen(str);
   return ret;
}

%}

%%
[0-9][0-9bkwy\s]*(.[0-9\s]+)? {yylval = new Node(NUMBER, readtoken(yytext));return NUMBER;}
$name_[0-9]*    {yylval = new Node(NAME, readtoken(yytext));return NAME;}
function        {yylval = new Node(FUNCTION, readtoken(yytext));return FUNCTION;}
func_begin      {yylval = new Node(FUNC_BEGIN, readtoken(yytext));return FUNC_BEGIN;}
func_end        {yylval = new Node(FUNC_END, readtoken(yytext));return FUNC_END;}
func_param      {yylval = new Node(FUNC_PARAM, readtoken(yytext));return FUNC_PARAM;}
return          {yylval = new Node(RETURN, readtoken(yytext));return RETURN;}
use_func        {yylval = new Node(USE_FUNC, readtoken(yytext));return USE_FUNC;}
use_to          {yylval = new Node(USE_TO, readtoken(yytext));return USE_TO;}
this_is         {yylval = new Node(THIS_IS, readtoken(yytext));return THIS_IS;}
def             {yylval = new Node(DEF, readtoken(yytext));return DEF;}
named_as        {yylval = new Node(NAMED_AS, readtoken(yytext));return NAMED_AS;}
list            {yylval = new Node(TYPE_LIST, readtoken(yytext));return TYPE_LIST;}
number          {yylval = new Node(TYPE_NUMBER, readtoken(yytext));return TYPE_NUMBER;}
string          {yylval = new Node(TYPE_STRING, readtoken(yytext));return TYPE_STRING;}
print_it        {yylval = new Node(PRINT_IT, readtoken(yytext));return PRINT_IT;}
\n              {n_line ++; n_char = 1;}
.               {n_char += strlen(yytext);}
%%

int yywrap() 
{ 
   return(1); 
} 