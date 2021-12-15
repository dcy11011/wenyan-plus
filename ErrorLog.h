#ifndef ERRORLOG
#define ERRORLOG

#include <iostream>
#include <string>

enum LogType{
    log_error,
    log_warning,
    log_info,
};

void wyLog(int type, const char *error_info);
void wyLog(int type, const std::string &error_info);
std::ostream& wyLog(int type);

#endif