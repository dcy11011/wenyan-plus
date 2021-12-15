%{
#include <stdio.h>  
#include <stdlib.h> 

#include "AST.h"
#include "ChineseConvert.h"
#include "ErrorLog.h"
#include "lex.yy.c"
#include "token.h"

int yyerror(const std::string &msg)
{
    wyLog(log_error)<<msg;
}
extern std::string token_name[2048];

Node *p, *root;

%}

%token NUMBER NAME
%token FUNCTION  FUNC_BEGIN FUNC_END FUNC_PARAM USE_FUNC USE_TO RETURN
%token THIS_IS DEF NAMED_AS
%token TYPE_LIST TYPE_NUMBER TYPE_STRING
%token PRINT_IT

%%
file : file section{root = $$;} | section {};
section : function | sentences ;
sentences : sentence | sentences sentence ;
sentence : value | return_sentence | print_sentence;
value : function_use ;
function_use : USE_FUNC NAME USE_TO params ;
return_sentence : RETURN NAME ;
print_sentence : sentence PRINT_IT ;
params : params NAME | params NUMBER | NAME | NUMBER ;
function : function_def sentences function_end ;
function_def : DEF FUNCTION NAMED_AS NAME function_params FUNC_BEGIN ;
function_params : FUNC_PARAM func_param_packs ;
func_param_packs : func_param_packs func_param_pack | func_param_pack ;
func_param_pack : NUMBER type name_defs ;
function_end : THIS_IS NAME FUNC_END ;
type : TYPE_NUMBER | TYPE_STRING | TYPE_LIST ;
name_defs : name_defs NAMED_AS NAME | NAMED_AS NAME ;

%%
 
int main()
{
    init_token();
    extern FILE* yyin;
    ChineseConverter chinese_converter;
    chinese_converter.loadConfig("parse.config");
    FILE *fin = fopen("test.wy","r");
    char c;
    std::vector<char> str;
    while((c=fgetc(fin))!=EOF){
        str.push_back(c);
    }
    str.push_back(0);
    auto p1 = chinese_converter.convertString(std::string(str.data()));
    FILE * filetmp = fopen("tmp.wytmp","w");
    fwrite(p1.c_str(), p1.size(), 1, filetmp);
    fclose(filetmp);
    yyin = fopen("tmp.wytmp","r");
    yyparse();
    root->printAll();
    return 0;
}