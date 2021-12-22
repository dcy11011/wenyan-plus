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

%token NUMBER NAME INF
%token FUNCTION  FUNC_BEGIN FUNC_END FUNC_PARAM USE_FUNC USE_TO RETURN RETURN_IT
%token THIS_IS DEF NAMED_AS DEF_AS DEFNAMED_AS
%token TYPE_NUMBER TYPE_STRING
%token PRINT_IT PRINT
%token WHILE_TRUE DO TIMES END BREAK FORIN CONTAINS
%token LOGIC_EQUAL LOGIC_LESS LOGIC_GREATER
%token IF_BEGIN IF_END IF_STAT IF_ELSE
%token GET IT LENGTH
%token ARI_ADD ARI_SUB ARI_MUL ARI_DIV ARI_MOD
%token EVAL ROUND_IT FLOOR_IT
%token LIST_E LIST_CONCAT LIST_SLICE TO WITH

%%
file : sections {root = $$;};
sections: section | sections section;
section : function | sentencei ;
sentences : sentence | sentences sentencei ;
sentence : control_sentence | print_sentence | do_it_sentence | function_sentence
         | loop_sentence | if_sentence 
         | concat_sentence | slice_sentence 
         | index_sentence | eval_sentence | define_sentence;
sentencei: sentence | assign_sentence;
value : func_use | NAME | NUMBER | LIST_E | INF;
valuei : IT | value;
valueref : IT | NAME;
expression_3 : valuei | valuei single_operator ;
expression_2 : expression_3 ari_operator_1 expression_2 | expression_3;
expression_1 : expression_2 ari_operator_0 expression_1 | expression_2;
expression_0 : expression_1 logic_operator expression_0 | expression_1;
logic_operator : LOGIC_EQUAL | LOGIC_LESS | LOGIC_GREATER ;
single_operator : LENGTH;
ari_operator_0 : ARI_ADD | ARI_SUB;
ari_operator_1 : ARI_MUL | ARI_DIV | ARI_MOD ;
func_use : USE_FUNC NAME params ;
control_sentence : return_sentence | BREAK ;
do_it_sentence : PRINT_IT | RETURN_IT | ROUND_IT | FLOOR_IT;
return_sentence : RETURN expression_0;
print_sentence : PRINT expression_0;
function_sentence : func_use;
loop_sentence : loop_statment sentences END ;
loop_statment : while_true_loop | do_times_loop | for_in_loop;
while_true_loop : WHILE_TRUE ;
do_times_loop : DO value TIMES ;
for_in_loop : FORIN expression_0 CONTAINS NAME IF_STAT;
param : NAME | NUMBER | IT;
params : params USE_TO param | USE_TO param ;
function : func_def sentences func_end ;
func_def : DEF FUNCTION DEFNAMED_AS NAME func_params ;
func_params : FUNC_PARAM func_param_packs ;
func_param_packs : func_param_packs func_param_pack | func_param_pack ;
func_param_pack : NUMBER type name_defs ;
func_end : THIS_IS NAME FUNC_END ;
type : TYPE_NUMBER | TYPE_STRING | LIST_E;
name_defs : name_defs name_def | name_def ;
name_def : NAMED_AS NAME ;
if_end : IF_END | END;
if_sentence : if_statment sentences if_end | if_statment sentences if_end IF_ELSE sentences if_end;
if_statment : IF_BEGIN expression_0 IF_STAT ;
index_sentence : GET expression_0 IT value;
eval_sentence : EVAL expression_0 | GET expression_0;
assign_sentence : NAMED_AS NAME;
define_sentence : DEF_AS NAME IF_END | DEFNAMED_AS NAME;
concat_sentence : LIST_CONCAT valueref WITH expression_0;
slice_sentence : LIST_SLICE value TO value;
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
    // std::cout<<"\nAST:\n"; root->printAll();
    std::string js_code = root->codeGenerate(0, 0);
    std::cout<<"code:"<<std::endl<<js_code<<std::endl;
    fwrite(js_code.c_str(), 1, js_code.size(), fout);
    return 0;
}