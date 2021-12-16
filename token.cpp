#include "token.h"
std::string token_name[2048];
int init_token(){
    token_name[258]=std::string("NUMBER");
    token_name[259]=std::string("NAME");
    token_name[260]=std::string("FUNCTION");
    token_name[261]=std::string("FUNC_BEGIN");
    token_name[262]=std::string("FUNC_END");
    token_name[263]=std::string("FUNC_PARAM");
    token_name[264]=std::string("USE_FUNC");
    token_name[265]=std::string("USE_TO");
    token_name[266]=std::string("RETURN");
    token_name[267]=std::string("THIS_IS");
    token_name[268]=std::string("DEF");
    token_name[269]=std::string("NAMED_AS");
    token_name[270]=std::string("TYPE_LIST");
    token_name[271]=std::string("TYPE_NUMBER");
    token_name[272]=std::string("TYPE_STRING");
    token_name[273]=std::string("PRINT_IT");
}
std::string getTokenName(int tok){return token_name[tok];}
