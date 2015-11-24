#ifndef __Lexer__
#define __Lexer__

#include <iostream>
#include <cstring>
#include <cstdlib>
#include "Includes/constants.h"
#include "Includes/Globals.h"
#include "Includes/Utility.h"
#include "KeywordManager.h"

using namespace std;


bool tokenWordMatchesKeyword(char *word, char *keyword)
{
    /* Convert to lowercase */
    char *caseInsensitiveToken = new char[strlen(word)];
    strcpy(caseInsensitiveToken, word);
    lowercase(caseInsensitiveToken);
    
    typedef std::map<char*, char*>::iterator it_type;
    for(it_type iterator = custom_keywords_table.begin(); iterator != custom_keywords_table.end(); iterator++) {
        // iterator->first = key
        // iterator->second = value
        if(strcmp(iterator->first, keyword) == 0){
            return (strcmp(caseInsensitiveToken, iterator->second) == 0);
        }
    }
    
    return false;
}

bool tokenCharactersMatchesKeyword(const char *source, int start_position, const char *keyword)
{
    char *customKeyword = getCustomKeywordForBaseKeyword(keyword);
    
    /* Check if the first characters match. */
    if(source[start_position] != customKeyword[0]){
        return false;
    }
    
    /* Check if subsequent characters match until a mismatch is found */
    for(int i = 1; i < strlen(customKeyword); i++){
        if(source[start_position+i] != customKeyword[i]){
            return false;
        }
    }
    
    /* Match */
    return true;
}

bool isDigit(char *str)
{
    if(strcmp(str,"0") == 0 ||
       strcmp(str,"1") == 0 ||
       strcmp(str,"2") == 0 ||
       strcmp(str,"3") == 0 ||
       strcmp(str,"4") == 0 ||
       strcmp(str,"5") == 0 ||
       strcmp(str,"6") == 0 ||
       strcmp(str,"7") == 0 ||
       strcmp(str,"8") == 0 ||
       strcmp(str,"9") == 0){
        return true;
    }
    else{
        return false;
    }
}

bool isArithmeticOperator(char *str)
{
    if(strcmp(str,"+") == 0 ||
       strcmp(str,"-") == 0 ||
       strcmp(str,"*") == 0 ||
       strcmp(str,"/") == 0 ||
       strcmp(str,"%") == 0/* ||
       strcmp(str,"(") == 0 ||
       strcmp(str,")") == 0*/){
        return true;
    }
    else{
        return false;
    }
}

bool isArithmeticOperator(char c)
{
    if(c == '+' ||
       c == '-' ||
       c == '*' ||
       c == '/' ||
       c == '%' ||
       c == '(' ||
       c == '-'){
        return true;
    }
    else{
        return false;
    }
}

char** lex(const char *filecontents, int* returnSize)
{
    int size = strlen(filecontents)-1;
    
    if(size > MAX_LANG_FILE_SIZE){
        // TODO: Handle file too large error
        return NULL;
    }
    
    char *token = new char[MAX_LANG_FILE_SIZE];
    char *lowercaseToken = new char[MAX_LANG_FILE_SIZE];
    char *stringLiteral = new char[MAX_STRING_SIZE];
    char *expressionLiteral = new char[MAX_STRING_SIZE];
    char *number = new char[MAX_IDENTIFIER_SIZE];
    char *var = new char[MAX_IDENTIFIER_SIZE];
    char *funcName = new char[MAX_IDENTIFIER_SIZE];
    char *funcRet = new char[MAX_IDENTIFIER_SIZE];
    char *funcArgType = new char[MAX_IDENTIFIER_SIZE];
    char *funcArgName = new char[MAX_IDENTIFIER_SIZE];
    
    bool isLookingAtString = false;
    bool isExpression = false;
    bool varStarted = false;
    //bool funcNameStarted = false;
    bool funcDeclStarted = false;
    bool funcRetStarted = false;
    bool funcArgTypeStarted = false;
    bool funcArgNameStarted = false;
    bool lineIsComment = false;
    
    char **tokenList = new char*[MAX_TOKEN_LIST_SIZE];
    int tokenListSize = 0;
    
    if(isPrintingEnabled){printf("File contains %d characters\n", size);}
    for(int i = 0; i < size; i++){
        /* increment token */
        appendChar(token, MAX_LANG_FILE_SIZE, filecontents[i]);
        char prevChar;
        if(i > 0){
            prevChar = filecontents[i-1];
        }
        else{
            prevChar = '\0';
        }
        if(isPrintingEnabled){printf("TOKEN: %s\n", token);}
        
        /* lex. Should reset the token variable each time we find a valid token. */
        if(strcmp(token," ") == 0 || strcmp(token,"\t") == 0){
            if(isLookingAtString != true){
                strcpy(token,"");
            }
        }
        else if(strcmp(token,"#") == 0 && !isLookingAtString){
            lineIsComment = true;
            strcpy(token,"");
        }
        else if(strcmp(token,";") == 0 || strcmp(token,"\n") == 0){
            lineIsComment = false;
            strcpy(token,"");
            if(strcmp(expressionLiteral,"") != 0 && isExpression){
                //printf("\t\tEXPR: %s\n", expressionLiteral);
                
                /* Create and add token for string literal */
                char strbuff[1000];
                sprintf(strbuff, "EXPR: %s", expressionLiteral);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                strcpy(expressionLiteral,"");
            }
            else if(strcmp(expressionLiteral,"") != 0 && !isExpression){

                /* Create and add token for string literal */
                char strbuff[1000];
                sprintf(strbuff, "NUM: %s", expressionLiteral);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                strcpy(expressionLiteral,"");
            }
            else if(strcmp(var,"") != 0){
                /* Create and add token for variable name */
                char strbuff[1000];
                sprintf(strbuff, "VAR: %s", var);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                /* Reset var */
                strcpy(var,"");
                varStarted = false;
            }

            // TODO: Uncomment for bnf stuff
//            tokenList[tokenListSize] = new char[1];
//            strcpy(tokenList[tokenListSize], ";");
//            tokenListSize++;
            
            isExpression = false;
        }
        else if(lineIsComment){
            // Do nothing
            strcpy(token,"");
        }
        else if(strcmp(token,"increment") == 0){
            tokenList[tokenListSize++] = "INCREMENT";
            strcpy(token,"");
        }
        else if(strcmp(token,"+") == 0 && prevChar == '+' && !varStarted){
            
            /* Create and add token for variable name */
            char strbuff[1000];
            sprintf(strbuff, "PLUSPLUS");
            tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
            strcpy(tokenList[tokenListSize], strbuff);
            tokenListSize++;
            
            /* Reset token */
            strcpy(token,"");
        }
        else if(tokenCharactersMatchesKeyword(filecontents, i, "assign") && isLookingAtString == false){

            /* Detect variable name and add to tokens list */
            if(strcmp(var,"") != 0){
                /* Create and add token for variable name */
                //printf("var: %s strlen: %d\n", var, strlen(var));
                //var[strlen(var)] = '\0';// COMMENT OUT IF ASSIGNMENT OPERTOR IS ONLY ONE CHARACTER
                char strbuff[1000];
                sprintf(strbuff, "VAR:%s", var);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                /* Reset var */
                strcpy(var,"");
                varStarted = false;
            }
            
            /* Create and add token for variable assignment */
            char strbuff[7];
            sprintf(strbuff, "ASSIGN");
            tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
            strcpy(tokenList[tokenListSize], strbuff);
            tokenListSize++;
            
            /* Reset token */
            strcpy(token,"");
            
            i+=(strlen(getCustomKeywordForBaseKeyword("assign"))-1);
        }
        else if(strcmp(token,"<") == 0 && filecontents[i+1] == '=' && isLookingAtString == false){
            
            /* Tokenise expression or variable */
            if(strcmp(expressionLiteral,"") != 0 && !isExpression){
                
                /* Create and add token for string literal */
                char strbuff[1000];
                sprintf(strbuff, "NUM: %s", expressionLiteral);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                strcpy(expressionLiteral,"");
            }
            else if(strcmp(var,"") != 0 && varStarted){
                /* Create and add token for variable name */
                char strbuff[1000];
                sprintf(strbuff, "VAR: %s", var);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                /* Reset var */
                strcpy(var,"");
                varStarted = false;
            }
            
            /* Create and add token for <= */
            char strbuff[7];
            sprintf(strbuff, "LESSOREQUALS");
            tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
            strcpy(tokenList[tokenListSize], strbuff);
            tokenListSize++;
            
            /* Reset token */
            strcpy(token,"");
        }
        else if(strcmp(token,">") == 0 && filecontents[i+1] == '=' && isLookingAtString == false){
            
            /* Tokenise expression or variable */
            if(strcmp(expressionLiteral,"") != 0 && !isExpression){
                
                /* Create and add token for string literal */
                char strbuff[1000];
                sprintf(strbuff, "NUM: %s", expressionLiteral);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                strcpy(expressionLiteral,"");
            }
            else if(strcmp(var,"") != 0 && varStarted){
                /* Create and add token for variable name */
                char strbuff[1000];
                sprintf(strbuff, "VAR: %s", var);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                /* Reset var */
                strcpy(var,"");
                varStarted = false;
            }
            
            /* Create and add token for >= */
            char strbuff[7];
            sprintf(strbuff, "GREATEROREQUALS");
            tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
            strcpy(tokenList[tokenListSize], strbuff);
            tokenListSize++;
            
            /* Reset token */
            strcpy(token,"");
        }
        else if(strcmp(token,"!") == 0 && filecontents[i+1] == '=' && isLookingAtString == false){
            
            /* Tokenise expression or variable */
            if(strcmp(expressionLiteral,"") != 0 && !isExpression){
                
                /* Create and add token for string literal */
                char strbuff[1000];
                sprintf(strbuff, "NUM: %s", expressionLiteral);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                strcpy(expressionLiteral,"");
            }
            else if(strcmp(var,"") != 0 && varStarted){
                /* Create and add token for variable name */
                char strbuff[1000];
                sprintf(strbuff, "VAR: %s", var);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                /* Reset var */
                strcpy(var,"");
                varStarted = false;
            }
            
            /* Create and add token for != */
            char strbuff[7];
            sprintf(strbuff, "NOTEQUALS");
            tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
            strcpy(tokenList[tokenListSize], strbuff);
            tokenListSize++;
            
            /* Reset token */
            strcpy(token,"");
        }
        else if(strcmp(token,"=") == 0 && isLookingAtString == false){
            
            /* Tokenise expression or variable */
            if(strcmp(expressionLiteral,"") != 0 && !isExpression){
                
                /* Create and add token for string literal */
                char strbuff[1000];
                sprintf(strbuff, "NUM: %s", expressionLiteral);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                strcpy(expressionLiteral,"");
            }
            else if(strcmp(var,"") != 0 && varStarted){
                /* Create and add token for variable name */
                char strbuff[1000];
                sprintf(strbuff, "VAR: %s", var);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                /* Reset var */
                strcpy(var,"");
                varStarted = false;
            }
            
            if(prevChar == '='){
                /* Create and add token for variable assignment */
                char strbuff[7];
                sprintf(strbuff, "EQUALITY");
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
            }
            
            /* Reset token */
            strcpy(token,"");
        }
        else if(strcmp(token,"<") == 0 && filecontents[i-1] != '<' && filecontents[i+1] != '<' && isLookingAtString == false){

            /*Tokenise expression or variable */
            if(strcmp(expressionLiteral,"") != 0 && !isExpression){
                
                /* Create and add token for string literal */
                char strbuff[1000];
                sprintf(strbuff, "NUM: %s", expressionLiteral);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                strcpy(expressionLiteral,"");
            }
            else if(strcmp(var,"") != 0 && varStarted){
                /* Create and add token for variable name */
                char strbuff[1000];
                sprintf(strbuff, "VAR: %s", var);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                /* Reset var */
                strcpy(var,"");
                varStarted = false;
            }
            
            /* Create and add token for != */
            char strbuff[7];
            sprintf(strbuff, "LESS");
            tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
            strcpy(tokenList[tokenListSize], strbuff);
            tokenListSize++;
            
            /* Reset token */
            strcpy(token,"");
        }
        else if(strcmp(token,">") == 0 && isLookingAtString == false){

            /* Tokenise expression or variable */
            if(strcmp(expressionLiteral,"") != 0 && !isExpression){
                
                /* Create and add token for string literal */
                char strbuff[1000];
                sprintf(strbuff, "NUM: %s", expressionLiteral);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                strcpy(expressionLiteral,"");
            }
            else if(strcmp(var,"") != 0 && varStarted){
                /* Create and add token for variable name */
                char strbuff[1000];
                sprintf(strbuff, "VAR: %s", var);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                /* Reset var */
                strcpy(var,"");
                varStarted = false;
            }
            
            /* Create and add token for != */
            char strbuff[7];
            sprintf(strbuff, "GREATER");
            tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
            strcpy(tokenList[tokenListSize], strbuff);
            tokenListSize++;
            
            /* Reset token */
            strcpy(token,"");
        }
        else if(tokenCharactersMatchesKeyword(filecontents, i, "AND") && isLookingAtString == false){
            /* Tokenise expression or variable */
            if(strcmp(expressionLiteral,"") != 0 && !isExpression){
                
                /* Create and add token for string literal */
                char strbuff[1000];
                sprintf(strbuff, "NUM: %s", expressionLiteral);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                strcpy(expressionLiteral,"");
            }
            else if(strcmp(var,"") != 0 && varStarted){
                /* Create and add token for variable name */
                char strbuff[1000];
                sprintf(strbuff, "VAR: %s", var);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                /* Reset var */
                strcpy(var,"");
                varStarted = false;
            }
            
            tokenList[tokenListSize++] = "&&";
            strcpy(token,"");
            i+=(strlen(getCustomKeywordForBaseKeyword("AND"))-1);
        }
        else if(tokenCharactersMatchesKeyword(filecontents, i, "OR") && isLookingAtString == false){
            /* Tokenise expression or variable */
            if(strcmp(expressionLiteral,"") != 0 && !isExpression){
                
                /* Create and add token for string literal */
                char strbuff[1000];
                sprintf(strbuff, "NUM: %s", expressionLiteral);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                strcpy(expressionLiteral,"");
            }
            else if(strcmp(var,"") != 0 && varStarted){
                /* Create and add token for variable name */
                char strbuff[1000];
                sprintf(strbuff, "VAR: %s", var);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                /* Reset var */
                strcpy(var,"");
                varStarted = false;
            }
            
            tokenList[tokenListSize++] = "||";
            strcpy(token,"");
            i+=(strlen(getCustomKeywordForBaseKeyword("OR"))-1);
        }
        else if(tokenWordMatchesKeyword(token, "CREATE_LIST") && !isLookingAtString){
            /* List constructor found */
            tokenList[tokenListSize++] = "MKLST";
            strcpy(token,"");
        }
        else if(tokenWordMatchesKeyword(token, "ADD_TO_LIST") && !isLookingAtString){
            /* List add function found */
            tokenList[tokenListSize++] = "LSTADD";
            strcpy(token,"");
        }
        else if(tokenWordMatchesKeyword(token, "REMOVE_FROM_LIST") && !isLookingAtString){
            /* List remove function found */
            tokenList[tokenListSize++] = "LSTRMV_IDX";
            strcpy(token,"");
        }
        else if(tokenWordMatchesKeyword(token, "PUT_IN_LIST") && !isLookingAtString){
            /* List put function found */
            tokenList[tokenListSize++] = "LSTPUT";
            strcpy(token,"");
        }
        else if(tokenWordMatchesKeyword(token, "disp") && !isLookingAtString){
            /* DISP FOUND */
            tokenList[tokenListSize++] = "DISP";
            strcpy(token,"");
        }
        else if(tokenWordMatchesKeyword(token, "endif") && !isLookingAtString){
            tokenList[tokenListSize++] = "ENDIF";
            strcpy(token,"");
        }
        else if(tokenWordMatchesKeyword(token, "if") && !isLookingAtString){
            tokenList[tokenListSize++] = "IF";
            strcpy(token,"");
        }
        else if(tokenCharactersMatchesKeyword(filecontents, i, "then") && !isLookingAtString){
            
            /* Make delimiter for == expression */
            if(strcmp(expressionLiteral,"") != 0 && !isExpression){
                
                /* Create and add token for string literal */
                char strbuff[1000];
                sprintf(strbuff, "NUM: %s", expressionLiteral);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                strcpy(expressionLiteral,"");
            }
            else if(strcmp(var,"") != 0 && varStarted){
                /* Create and add token for variable name */
                char strbuff[1000];
                sprintf(strbuff, "VAR: %s", var);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                /* Reset var */
                strcpy(var,"");
                varStarted = false;
            }
        }
        else if(tokenWordMatchesKeyword(token, "then") && !isLookingAtString){
            tokenList[tokenListSize++] = "THEN";
            strcpy(token,"");
        }
        else if(tokenWordMatchesKeyword(token, "else") && !isLookingAtString){
            tokenList[tokenListSize++] = "ELSE";
            strcpy(token,"");
        }
        else if(tokenWordMatchesKeyword(token, "elif") && !isLookingAtString){
            tokenList[tokenListSize++] = "ELSEIF";
            strcpy(token,"");
        }
        else if(tokenWordMatchesKeyword(token, "while") && !isLookingAtString){
            tokenList[tokenListSize++] = "WHILE";
            strcpy(token,"");
        }
        else if(tokenWordMatchesKeyword(token, "endwhile") && !isLookingAtString){
            tokenList[tokenListSize++] = "ENDWHILE";
            strcpy(token,"");
        }
        else if(isDigit(token) == true && !isLookingAtString){
            strcat(expressionLiteral, token);
            strcpy(token,"");
        }
        else if(isArithmeticOperator(token) == true && !isLookingAtString){
            isExpression = true;
            strcat(expressionLiteral, token);
            strcpy(token,"");
        }
        else if(strcmp(token,"\"") == 0){
            if(isLookingAtString == true){
                /* End of string literal */
                //printf("STRING FOUND\n");
                
                /* Add closing quote to string literal */
                strcat(stringLiteral, "\"");
                
                /* Create and add token for string literal */
                char strbuff[1000];
                sprintf(strbuff, "STRING: %s", stringLiteral);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                /* Reset */
                strcpy(stringLiteral,"");
                strcpy(token,"");
                isLookingAtString = false;
            }
            else{
                /* Start of string literal */
                isLookingAtString = true;
            }
        }
        else if(isLookingAtString == true){
            //appendChar(stringLiteral, MAX_STRING_SIZE, filecontents[i]);
            strcat(stringLiteral, token);
            strcpy(token,"");
        }
        else if(strcmp(token,variable_syntax_denoter) == 0 && !isLookingAtString){
            varStarted = true;
            strcat(var, token);
            strcpy(token,"");
        }
        else if(tokenWordMatchesKeyword(token, "return") && !isLookingAtString){
            tokenList[tokenListSize++] = "RETURN";
            strcpy(token,"");
            strcpy(funcName,"");
            funcDeclStarted = false;
        }
        else if(tokenWordMatchesKeyword(token, "endfunc") && !isLookingAtString){
            tokenList[tokenListSize++] = "FUNCTION_CLOSE";
            strcpy(token,"");
            strcpy(funcName,"");
            funcDeclStarted = false;
        }
        else if(tokenWordMatchesKeyword(token, "func") && !isLookingAtString){
            tokenList[tokenListSize++] = "FUNCTION_DECL";
            strcpy(token,"");
            funcDeclStarted = true;
            funcArgTypeStarted = false;
        }
        else if(strcmp(token,"(") == 0 && !isLookingAtString && funcDeclStarted){
            /* Create and add token for function name */
            char strbuff[1000];
            sprintf(strbuff, "%s", funcName);
            tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
            strcpy(tokenList[tokenListSize], strbuff);
            tokenListSize++;
            
            /* Reset */
            strcpy(funcName,"");
            strcpy(token,"");
            //funcDeclStarted = false;
            funcArgTypeStarted = true;
            strcpy(funcArgType,"");
        }
        else if(strcmp(token,":") == 0 && !isLookingAtString && funcArgTypeStarted){
            /* Prepare to start lexing arg name */
            funcArgTypeStarted = false;
            funcArgNameStarted = true;
            strcpy(funcArgName,"");
            strcpy(token,"");
        }
        else if(strcmp(token,",") == 0 && !isLookingAtString && funcArgNameStarted){
            funcArgTypeStarted = true;
            funcArgNameStarted = false;
            
            /* Create and add token for function argument */
            char strbuff[1000];
            sprintf(strbuff, "ARG=%s:%s", funcArgType, funcArgName);
            tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
            strcpy(tokenList[tokenListSize], strbuff);
            tokenListSize++;
            
            /* Reset */
            strcpy(funcArgName,"");
            strcpy(funcArgType,"");
            strcpy(token,"");
        }
        else if(strcmp(token,")") == 0 && !isLookingAtString && funcDeclStarted){
            /* Append closing bracket */
            //strcat(funcName, token);
            
            /* Create and add token for function argument */
            char strbuff[1000];
            sprintf(strbuff, "ARG=%s:%s", funcArgType, funcArgName);
            tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
            strcpy(tokenList[tokenListSize], strbuff);
            tokenListSize++;
            
            /* Reset */
            strcpy(funcName,"");
            strcpy(funcArgName,"");
            strcpy(funcArgType,"");
            strcpy(token,"");
            funcDeclStarted = false;
            funcArgNameStarted = false;
            funcRetStarted = true;
        }
        else if(filecontents[i] == ')' && !isLookingAtString){
            /* Create and add token for function call */
            char strbuff[1000];
            sprintf(strbuff, "CALL:%s", token);
            tokenList[tokenListSize] = new char[100];
            strcpy(tokenList[tokenListSize], strbuff);
            tokenListSize++;
            
            /* Reset */
            strcpy(token,"");
        }
        else if(filecontents[i+1] == ':' && !isLookingAtString && funcRetStarted){
            /* Create and add token for function return type */
            char strbuff[1000];
            sprintf(strbuff, "RET:%s", token);
            tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
            strcpy(tokenList[tokenListSize], strbuff);
            tokenListSize++;
            
            /* Reset */
            strcpy(token,"");
            funcRetStarted = false;
            
            /* Skip next char which will be ":" */
            i++;
        }
        else if(varStarted){
            strcat(var, token);
            strcpy(token,"");
        }
        else if(funcDeclStarted && funcArgTypeStarted){
            strcat(funcArgType, token);
            strcpy(token,"");
            //printf("ARGTYPE: %s ", funcArgType);
        }
        else if(funcDeclStarted && funcArgNameStarted){
            strcat(funcArgName, token);
            strcpy(token,"");
            //printf("ARGNAME: %s ", funcArgName);
        }
        else if(funcDeclStarted && !funcRetStarted){
            strcat(funcName, token);
            strcpy(token,"");
            //printf("FUNCNAME: %s ", funcName);
        }
        
        strcpy(lowercaseToken, token);
        lowercase(lowercaseToken);
    }
    
    /* Cleanup */
    delete [] token;
    delete [] lowercaseToken;
    delete [] stringLiteral;
    delete [] expressionLiteral;
    delete [] number;
    delete [] var;
    delete [] funcName;
    delete [] funcRet;
    delete [] funcArgType;
    delete [] funcArgName;
    
    *returnSize = tokenListSize;
    return tokenList;
}



#endif
