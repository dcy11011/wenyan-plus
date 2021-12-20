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
%token PRINT_IT PRINT
%token WHILE_TRUE DO TIMES END

%%
file : file section{root = $$;} | section {};
section : function | sentences ;
sentences : sentence | sentences sentence ;
sentence : return_sentence | print_sentence | function_sentence | loop_sentence;
value : func_use | NAME | NUMBER;
func_use : USE_FUNC NAME USE_TO params ;
return_sentence : RETURN NAME ;
print_sentence : value PRINT_IT | PRINT value;
function_sentence : func_use;
loop_sentence : loop_statment sentences END ;
loop_statment : while_true_loop | do_times_loop;
while_true_loop : WHILE_TRUE ;
do_times_loop : DO NAME TIMES | DO value TIMES ;
params : params NAME | params NUMBER | NAME | NUMBER ;
function : func_def sentences func_end ;
func_def : DEF FUNCTION NAMED_AS NAME func_params FUNC_BEGIN ;
func_params : FUNC_PARAM func_param_packs ;
func_param_packs : func_param_packs func_param_pack | func_param_pack ;
func_param_pack : NUMBER type name_defs ;
func_end : THIS_IS NAME FUNC_END ;
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
    FILE *fout= fopen("test.js","w");
    char c;
    std::vector<char> str;
    while((c=fgetc(fin))!=EOF){
        str.push_back(c);
    }
    str.push_back(0);
    auto p1 = chinese_converter.convertString(std::string(str.data()));
    std::cout<<"ascii code:"<<std::endl;
    std::cout<<p1<<std::endl;
    FILE * filetmp = fopen("tmp.wytmp","w");
    fwrite(p1.c_str(), p1.size(), 1, filetmp);
    fclose(filetmp);
    yyin = fopen("tmp.wytmp","r");
    yyparse();
    std::cout<<"\nAST:\n";
    root->printAll();
    std::string js_code = root->codeGenerate();
    std::cout<<"code:"<<std::endl<<js_code<<std::endl;
    fwrite(js_code.c_str(), 1, js_code.size(), fout);
    return 0;
}