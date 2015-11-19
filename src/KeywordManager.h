#ifndef __Keyword_Manager__
#define __Keyword_Manager__

#include <map>
#include "Includes/constants.h"
#include "Includes/Utility.h"


std::map<char*, char*> custom_keywords_table;

void initialiseKeywordsTable()
{
    custom_keywords_table["if"] = "if";
    custom_keywords_table["then"] = "then";
    custom_keywords_table["else"] = "else";
    custom_keywords_table["elif"] = "elif";
    custom_keywords_table["endif"] = "endif";
    custom_keywords_table["while"] = "while";
    custom_keywords_table["endwhile"] = "endwhile";
    custom_keywords_table["assign"] = "<<";
    custom_keywords_table["AND"] = "AND";
    custom_keywords_table["OR"] = "OR";
    custom_keywords_table["EQUALS"] = "==";
    custom_keywords_table["disp"] = "disp";
    custom_keywords_table["return"] = "return";
    custom_keywords_table["func"] = "func";
    custom_keywords_table["endfunc"] = "endfunc";
    
    custom_keywords_table["increment"] = "increment";
    custom_keywords_table["decrement"] = "decrement";
    custom_keywords_table["CREATE_LIST"] = "create_list"; // function remove(type, size)
    custom_keywords_table["ADD_TO_LIST"] = "add_to_list"; // function add(list, value)
    custom_keywords_table["PUT_IN_LIST"] = "put_in_list"; // function put(list, index, value)
    custom_keywords_table["REMOVE_FROM_LIST"] = "remove_from_position_in_list"; // function remove(list, index)
}

void updateKeywordTable(const char *keyword, const char *customKeyword)
{
    typedef std::map<char*, char*>::iterator it_type;
    for(it_type iterator = custom_keywords_table.begin(); iterator != custom_keywords_table.end(); iterator++) {
        if(strcmp(iterator->first, keyword) == 0){
            iterator->second = (char*) customKeyword;
        }
    }
}

char* getCustomKeywordForBaseKeyword(const char *baseKeyword)
{
    typedef std::map<char*, char*>::iterator it_type;
    for(it_type iterator = custom_keywords_table.begin(); iterator != custom_keywords_table.end(); iterator++) {
        if(strcmp(iterator->first, baseKeyword) == 0){
            return iterator->second;
        }
    }
    
    return "";
}

void loadKeywordsFromConfigFile()
{
    initialiseKeywordsTable();
    
    char *configFileContents = readFile(KEYWORD_CONFIG_FILE_PATH);
    char **lines = str_split(configFileContents, '\n');
    
    for(int i = 0; i < KEYWORD_COUNT; i++){
        char *line = lines[i];

        char **lineSplit = str_split(line, '%');
        char *keyword = lineSplit[0];
        char *customKeyword = new char[50];
        strcpy(customKeyword, lineSplit[1]);
        updateKeywordTable(keyword, customKeyword);
        
        free(lineSplit[0]);
        free(lineSplit[1]);
        free(lineSplit);
        free(lines[i]);
    }
    
    free(lines);
}

void updateKeywordsTableFromCommandLine(int argc, const char * argv[])
{
    /* Loop through each command line argument */
    for (int i=1; i < argc-1; i++){
        
        const char *arg = argv[i];
        
        if(strcmp(arg, "-if") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("if", arg);
        }
        else if(strcmp(arg, "-then") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("then", arg);
        }
        else if(strcmp(arg, "-else") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("else", arg);
        }
        else if(strcmp(arg, "-elif") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("elif", arg);
        }
        else if(strcmp(arg, "-endif") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("endif", arg);
        }
        else if(strcmp(arg, "-while") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("while", arg);
        }
        else if(strcmp(arg, "-ewhile") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("endwhile", arg);
        }
        else if(strcmp(arg, "-assgn") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("assign", arg);
        }
        else if(strcmp(arg, "-and") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("AND", arg);
        }
        else if(strcmp(arg, "-or") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("OR", arg);
        }
        else if(strcmp(arg, "-e") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("EQUALS", arg);
        }
        else if(strcmp(arg, "-disp") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("disp", arg);
        }
        else if(strcmp(arg, "-ret") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("return", arg);
        }
        else if(strcmp(arg, "-func") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("func", arg);
        }
        else if(strcmp(arg, "-efunc") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("endfunc", arg);
        }
        else if(strcmp(arg, "-lmake") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("CREATE_LIST", arg);
        }
        else if(strcmp(arg, "-ladd") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("ADD_TO_LIST", arg);
        }
        else if(strcmp(arg, "-lput") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("PUT_IN_LIST", arg);
        }
        else if(strcmp(arg, "-lrmv") == 0){
            i+=1;
            arg = argv[i];
            updateKeywordTable("REMOVE_FROM_LIST", arg);
        }
    }
}

#endif