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
    wyLog(log_error)<<msg<<" "<<yytext;
    return 1;
}
extern std::string token_name[2048];

Node *p, *root;

%}

%token NUMBER NAME
%token FUNCTION  FUNC_BEGIN FUNC_END FUNC_PARAM USE_FUNC USE_TO RETURN RETURN_IT
%token THIS_IS DEF NAMED_AS
%token TYPE_LIST TYPE_NUMBER TYPE_STRING
%token PRINT_IT PRINT
%token WHILE_TRUE DO TIMES END BREAK
%token LOGIC_EQUAL LOGIC_LESS LOGIC_GREATER
%token IF_BEGIN IF_END IF_STAT IF_ELSE
%token GET IT
%token EVAL

%%
file : sections {root = $$;};
sections: section | sections section;
section : function | sentencei ;
sentences : sentence | sentences sentencei ;
sentence : control_sentence | print_sentence | do_it_sentence | function_sentence
         | loop_sentence | if_sentence | get_sentence
         | index_sentence | eval_sentence;
sentencei: sentence | assign_sentence;
value : func_use | NAME | NUMBER ;
valuei : IT | value;
expression: valuei | logic_statement ;
logic_statement : value logic_operator expression ;
logic_operator : LOGIC_EQUAL | LOGIC_LESS | LOGIC_GREATER ;
func_use : USE_FUNC NAME params ;
control_sentence : return_sentence | BREAK ;
do_it_sentence : PRINT_IT | RETURN_IT;
return_sentence : RETURN NAME | RETURN NUMBER;
print_sentence : PRINT expression;
function_sentence : func_use;
loop_sentence : loop_statment sentences END ;
loop_statment : while_true_loop | do_times_loop;
while_true_loop : WHILE_TRUE ;
do_times_loop : DO value TIMES ;
param : NAME | NUMBER | IT;
params : params USE_TO param | USE_TO param ;
function : func_def sentences func_end ;
func_def : DEF FUNCTION NAMED_AS NAME func_params ;
func_params : FUNC_PARAM func_param_packs ;
func_param_packs : func_param_packs func_param_pack | func_param_pack ;
func_param_pack : NUMBER type name_defs ;
func_end : THIS_IS NAME FUNC_END ;
type : TYPE_NUMBER | TYPE_STRING | TYPE_LIST ;
name_defs : name_defs name_def | name_def ;
name_def : NAMED_AS NAME ;
if_sentence : if_statment sentences IF_END | if_statment sentences IF_END IF_ELSE sentences IF_END;
if_statment : IF_BEGIN logic_statement IF_STAT ;
get_sentence : GET NAME ;
index_sentence : GET value IT value;
eval_sentence : EVAL expression;
assign_sentence : NAMED_AS NAME;
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
    std::string js_code = root->codeGenerate(0, 0);
    std::cout<<"code:"<<std::endl<<js_code<<std::endl;
    fwrite(js_code.c_str(), 1, js_code.size(), fout);
    return 0;
}