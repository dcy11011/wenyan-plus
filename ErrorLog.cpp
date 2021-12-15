#include "ErrorLog.h"

const char* wyLogTypeString(int type){
    switch (type){
        case log_error:
            return "[ERROR]";
            break;
        case log_warning:
            return "[WARNING]";
            break;
        case log_info:
            return "[INFO]";
            break;
        default:
            return "[UNKNOWN]";
    }
}

void wyLogContentPrint(const char *content){
    std::cerr<<content;
}

void wyLog(int type, const char *error_info){
    std::cerr << wyLogTypeString(type);
    wyLogContentPrint(error_info);
    std::cerr<<std::endl;
}

void wyLog(int type, const std::string &error_info){
    wyLog(type, error_info.c_str());
}

std::ostream& wyLog(int type){
    return std::cerr<<wyLogTypeString(type);
}