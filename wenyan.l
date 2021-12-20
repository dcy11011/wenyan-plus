%{
#include <iostream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <stack>
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

long long parseNumberString(const std::string &str){
	std::stack<long long> sta;
	for(auto i:str){
      if(i=='0')continue;
		if(isdigit(i))sta.push((int)(i-'0'));
		else if(i=='s'){
			if(sta.empty()) sta.push(10);
			else{
				long long t = sta.top();
                if(t<=10) sta.pop(), sta.push(t*10);
			}
		}
      char token[4] = {'b','k','w','y'};
      long long val[4] = {100,1000,10000,100000000};
      for(int j=0;j<4;j++)if(i==token[j]){
         long long t =0 ;
         while(!sta.empty() && sta.top()<val[j]) t+=sta.top(), sta.pop();
         sta.push(t*val[j]);
      }
	}
   long long ret = 0;
   while(!sta.empty()){
      ret += sta.top();
      sta.pop();
   }
   return ret;
}

%}

%%
[0-9][0-9bskwy\s]*(.[0-9\s]+)? {yylval = new Node(NUMBER, readtoken(yytext)); yylval->setVal(parseNumberString(yytext)); return NUMBER;}
$name_[0-9]*    {yylval = new Node(NAME, readtoken(yytext)); yylval->setStr(yytext); return NAME;}
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
print           {yylval = new Node(PRINT, readtoken(yytext));return PRINT;}
do              {yylval = new Node(DO, readtoken(yytext)); return DO;}
while_true      {yylval = new Node(WHILE_TRUE, readtoken(yytext)); return WHILE_TRUE;}
times           {yylval = new Node(TIMES, readtoken(yytext)); return TIMES;}
end             {yylval = new Node(END, readtoken(yytext)); return END;}
break           {yylval = new Node(BREAK, readtoken(yytext)); return BREAK;}
\n              {n_line ++; n_char = 1;}
.               {n_char += strlen(yytext);}
%%

int yywrap() 
{ 
   return(1); 
} 