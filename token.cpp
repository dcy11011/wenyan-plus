#include "token.h"
std::string token_name[2048];
int init_token(){
    token_name[258]=std::string("NUMBER");
    token_name[259]=std::string("NAME");
    token_name[260]=std::string("INF");
    token_name[261]=std::string("FUNCTION");
    token_name[262]=std::string("FUNC_BEGIN");
    token_name[263]=std::string("FUNC_END");
    token_name[264]=std::string("FUNC_PARAM");
    token_name[265]=std::string("USE_FUNC");
    token_name[266]=std::string("USE_TO");
    token_name[267]=std::string("RETURN");
    token_name[268]=std::string("RETURN_IT");
    token_name[269]=std::string("THIS_IS");
    token_name[270]=std::string("DEF");
    token_name[271]=std::string("NAMED_AS");
    token_name[272]=std::string("DEF_AS");
    token_name[273]=std::string("DEFNAMED_AS");
    token_name[274]=std::string("TYPE_NUMBER");
    token_name[275]=std::string("TYPE_STRING");
    token_name[276]=std::string("PRINT_IT");
    token_name[277]=std::string("PRINT");
    token_name[278]=std::string("WHILE_TRUE");
    token_name[279]=std::string("DO");
    token_name[280]=std::string("TIMES");
    token_name[281]=std::string("END");
    token_name[282]=std::string("BREAK");
    token_name[283]=std::string("FORIN");
    token_name[284]=std::string("CONTAINS");
    token_name[285]=std::string("LOGIC_EQUAL");
    token_name[286]=std::string("LOGIC_LESS");
    token_name[287]=std::string("LOGIC_GREATER");
    token_name[288]=std::string("IF_BEGIN");
    token_name[289]=std::string("IF_END");
    token_name[290]=std::string("IF_STAT");
    token_name[291]=std::string("IF_ELSE");
    token_name[292]=std::string("GET");
    token_name[293]=std::string("IT");
    token_name[294]=std::string("LENGTH");
    token_name[295]=std::string("ARI_ADD");
    token_name[296]=std::string("ARI_SUB");
    token_name[297]=std::string("ARI_MUL");
    token_name[298]=std::string("ARI_DIV");
    token_name[299]=std::string("ARI_MOD");
    token_name[300]=std::string("EVAL");
    token_name[301]=std::string("ROUND_IT");
    token_name[302]=std::string("FLOOR_IT");
    token_name[303]=std::string("LIST_E");
    token_name[304]=std::string("LIST_CONCAT");
    token_name[305]=std::string("LIST_SLICE");
    token_name[306]=std::string("TO");
    token_name[307]=std::string("WITH");
return 0;
}
std::string getTokenName(int tok){return token_name[tok];}
