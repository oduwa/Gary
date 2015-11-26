#ifndef __Globals__
#define __Globals__

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <map>

using namespace std;

char *variable_syntax_denoter = "@";
char *assignment_syntax_denoter = "<<";

FILE *main_output;
FILE *functions_output;
std::map<char*, char*> symbols_type_table;
std::map<char*, char*> function_symbol_table;
bool isPrintingEnabled = false;

#endif