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
file : file section{ $$=makeNode("file",$1,$2);} | section { $$=makeNode("file",$1);};
section : function  {$$=makeNode("section",$1);}| sentences  {$$=makeNode("section",$1);};
sentences : sentence  {$$=makeNode("sentences",$1);}| sentences sentence  {$$=makeNode("sentences",$1,$2);};
sentence : value  {$$=makeNode("sentence",$1);}| return_sentence  {$$=makeNode("sentence",$1);}| print_sentence {$$=makeNode("sentence",$1);};
value : function_use  {$$=makeNode("value",$1);};
function_use : USE_FUNC NAME USE_TO params  {$$=makeNode("function_use",$1,$2,$3,$4);};
return_sentence : RETURN NAME  {$$=makeNode("return_sentence",$1,$2);};
print_sentence : sentence PRINT_IT  {$$=makeNode("print_sentence",$1,$2);};
params : params NAME  {$$=makeNode("params",$1,$2);}| params NUMBER  {$$=makeNode("params",$1,$2);}| NAME  {$$=makeNode("params",$1);}| NUMBER  {$$=makeNode("params",$1);};
function : function_def sentences function_end  {$$=makeNode("function",$1,$2,$3);};
function_def : DEF FUNCTION NAMED_AS NAME function_params FUNC_BEGIN  {$$=makeNode("function_def",$1,$2,$3,$4,$5,$6);};
function_params : FUNC_PARAM func_param_packs  {$$=makeNode("function_params",$1,$2);};
func_param_packs : func_param_packs func_param_pack  {$$=makeNode("func_param_packs",$1,$2);}| func_param_pack  {$$=makeNode("func_param_packs",$1);};
func_param_pack : NUMBER type name_defs  {$$=makeNode("func_param_pack",$1,$2,$3);};
function_end : THIS_IS NAME FUNC_END  {$$=makeNode("function_end",$1,$2,$3);};
type : TYPE_NUMBER  {$$=makeNode("type",$1);}| TYPE_STRING  {$$=makeNode("type",$1);}| TYPE_LIST  {$$=makeNode("type",$1);};
name_defs : name_defs NAMED_AS NAME  {$$=makeNode("name_defs",$1,$2,$3);}| NAMED_AS NAME  {$$=makeNode("name_defs",$1,$2);};

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
    return 0;
}