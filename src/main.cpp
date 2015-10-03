//
//  main.cpp
//  MyLanguage
//
//  Created by Odie Edo-Osagie on 28/09/2015.
//  Copyright © 2015 Odie Edo-Osagie. All rights reserved.
//

#include <iostream>
#include <assert.h>
#include <map>

using namespace std;

static int MAX_LANG_FILE_SIZE = 5000000;
static int MAX_STRING_SIZE = 100000;
static int MAX_DIGIT_SIZE = 10;
static int MAX_TOKEN_LIST_SIZE = 100000;
static int MAX_EXPRESSION_SIZE = 200;
static int MAX_OPERATOR_SIZE = 20;

char *variable_syntax_denoter = "@";
char *assignment_syntax_denoter = "<<";

FILE * oFile;
std::map<char*, char*> symbols_type_table;

//returns 0 if failed, 1 if succeeded
int appendChar(char* s, size_t size, char c) {
    if(strlen(s) + 1 >= size) {
        return 0;
    }
    int len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
    return 1;
}

void lowercase(char *str)
{
    for(int i = 0; str[i]; i++){
        str[i] = (char) std::tolower(str[i]);
    }
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
       strcmp(str,"%") == 0 ||
       strcmp(str,"(") == 0 ||
       strcmp(str,")") == 0){
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

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;
    
    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }
    
    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);
    
    /* Add space for terminating null string so caller
     knows where the list of returned strings ends. */
    count++;
    
    result = (char **) malloc(sizeof(char*) * count);
    
    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);
        
        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    
    return result;
}

char* lookupVariableType(char* varname)
{
    typedef std::map<char*, char*>::iterator it_type;
    for(it_type iterator = symbols_type_table.begin(); iterator != symbols_type_table.end(); iterator++) {
        // iterator->first = key
        // iterator->second = value
        if(strcmp(iterator->first, varname) == 0){
            return iterator->second;
        }
    }
}

bool isVariableDefined(char *varname)
{
    typedef std::map<char*, char*>::iterator it_type;
    for(it_type iterator = symbols_type_table.begin(); iterator != symbols_type_table.end(); iterator++) {
        // iterator->first = key
        // iterator->second = value
        if(strcmp(iterator->first, varname) == 0){
            return true;
        }
    }
    
    return false;
}

char* readFile(const char *filename)
{
    /* Open File */
    FILE *f = fopen(filename, "rb");
    if(f == NULL){
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    /* Create string. Note the +1 for the null terminator */
    char *string = (char*) malloc(fsize + 1);
    fread(string, fsize, 1, f);
    fclose(f);
    
    /* Add null terminator */
    string[fsize] = '\0';
    
    return string;
}

void openOutputFile()
{
    oFile = fopen("language_run.cpp","w");
    
    fputs("#include <iostream>\n#include \"Includes/constants.h\"\n\nint main(){\n\n", oFile);
    fputs("printf(\"%sGARY:\\n\", KMAG);\n\n", oFile);
    fputs("long expression;\n", oFile);
}

void closeOutputFile()
{
    fputs("printf(\"%s\\n\", KNRM);\n\n", oFile);
    fputs("\n}", oFile);
    fclose(oFile);
}

char* evaluateExpression(const char *expr)
{
    char **num_stack = new char*[MAX_EXPRESSION_SIZE];
    int num_stack_size = 0;
    
    int i = 0;
    char *num = new char[MAX_DIGIT_SIZE];
    
    while(i <= strlen(expr)){
        char c = expr[i];
        
        if(isArithmeticOperator(c)){
            printf("%c\n",c);
            
            /* Append num */
            char *numCopy = new char[MAX_DIGIT_SIZE];
            strcpy(numCopy, num);
            num_stack[num_stack_size] = numCopy; num_stack_size++;
            
            /* Append operator */
            char *operatorString = new char[MAX_OPERATOR_SIZE];
            operatorString[0] = c;
            operatorString[1] = '\0';
            num_stack[num_stack_size] = operatorString; num_stack_size++;
            
            /* Reset num */
            strcpy(num,"");
        }
        else if(i == strlen(expr)){
            /* Append num */
            char *numCopy = new char[MAX_DIGIT_SIZE];
            strcpy(numCopy, num);
            num_stack[num_stack_size] = numCopy; num_stack_size++;
            
            /* Reset num */
            strcpy(num,"");
        }
        else{
            printf("NUMBER\n");
            appendChar(num, MAX_DIGIT_SIZE, c);
        }
        
        i++;
    }
    
    printf("NUM_STACK: ");
    for(int i = 0; i < num_stack_size; i++){
        printf("|%s| ", num_stack[i]);
    }
    printf("\n");
    
    delete [] num;
    delete [] num_stack;
    return "Got it";
    
//    Py_SetProgramName("Gary");  /* optional but recommended */
//    Py_Initialize();
//    PyRun_SimpleString("from time import time,ctime\n"
//                       "print 'Today is',ctime(time())\n");
//    Py_Finalize();
}

void evaluateExpressionInC(const char *expr)
{
    char line[100];
    sprintf(line, "expression = %s;\n", expr);
    fputs(line, oFile);
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
    char *number = new char[MAX_STRING_SIZE];
    char *var = new char[MAX_STRING_SIZE];
    
    bool isLookingAtString = false;
    bool isExpression = false;
    bool varStarted = false;
    
    char **tokenList = new char*[MAX_TOKEN_LIST_SIZE];
    int tokenListSize = 0;
    
    printf("File contains %d characters\n", size);
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
        printf("TOKEN: %s\n", token);
        
        /* lex. Should reset the token variable each time we find a valid token. */
        if(strcmp(token," ") == 0 || strcmp(token,"\t") == 0){
            if(isLookingAtString != true){
                strcpy(token,"");
            }
        }
        else if(strcmp(token,";") == 0 || strcmp(token,"\n") == 0){
            strcpy(token,"");
            if(strcmp(expressionLiteral,"") != 0 && isExpression){
                printf("\t\tEXPR: %s\n", expressionLiteral);
                
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
            
            isExpression = false;
        }
        else if(strcmp(token,"increment") == 0){
            tokenList[tokenListSize++] = "INCREMENT";
            strcpy(token,"");
        }
        else if(strcmp(token,"+") == 0 && prevChar == '+' && !varStarted){
            
            printf("XXXXXX\n");
            
            /* Create and add token for variable name */
            char strbuff[1000];
            sprintf(strbuff, "PLUSPLUS");
            tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
            strcpy(tokenList[tokenListSize], strbuff);
            tokenListSize++;
            
            /* Reset token */
            strcpy(token,"");
        }
        else if(strcmp(token,"<") == 0 && prevChar == '<' && isLookingAtString == false){
            printf("\t1\n");
            /* Detect variable name and add to tokens list */
            if(strcmp(var,"") != 0){
                /* Create and add token for variable name */
                //printf("var: %s strlen: %d\n", var, strlen(var));
                var[strlen(var)-1] = '\0';// COMMENT OUT IF ASSIGNMENT OPERTOR IS ONLY ONE CHARACTER
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
            
        }
        else if(strcmp(token,"<") == 0 && filecontents[i+1] == '=' && isLookingAtString == false){
            
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
            
            /* Create and add token for != */
            char strbuff[7];
            sprintf(strbuff, "GREATER");
            tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
            strcpy(tokenList[tokenListSize], strbuff);
            tokenListSize++;
            
            /* Reset token */
            strcpy(token,"");
        }
        else if(strcmp(token,"disp") == 0){
            /* DISP FOUND */
            tokenList[tokenListSize++] = "DISP";
            strcpy(token,"");
        }
        else if(strcmp(lowercaseToken,"disp") == 0){
            /* DISP FOUND */
            // TODO: Maybe display compile error
            tokenList[tokenListSize++] = "DISP";
            strcpy(token,"");
        }
        else if(strcmp(token,"endif") == 0){
            tokenList[tokenListSize++] = "ENDIF";
            strcpy(token,"");
        }
        else if(strcmp(token,"if") == 0){
            tokenList[tokenListSize++] = "IF";
            strcpy(token,"");
        }
        else if(filecontents[i] == 't' && filecontents[i+1] == 'h' && filecontents[i+2] == 'e' && filecontents[i+3] == 'n'){
            
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
        else if(strcmp(token,"then") == 0){
            tokenList[tokenListSize++] = "THEN";
            strcpy(token,"");
        }
        else if(strcmp(token,"else") == 0){
            tokenList[tokenListSize++] = "ELSE";
            strcpy(token,"");
        }
        else if(strcmp(token,"elif") == 0){
            tokenList[tokenListSize++] = "ELSEIF";
            strcpy(token,"");
        }
        else if(strcmp(token,"while") == 0){
            tokenList[tokenListSize++] = "WHILE";
            strcpy(token,"");
        }
        else if(strcmp(token,"endwhile") == 0){
            tokenList[tokenListSize++] = "ENDWHILE";
            strcpy(token,"");
        }
        else if(isDigit(token) == true){
            strcat(expressionLiteral, token);
            strcpy(token,"");
        }
        else if(isArithmeticOperator(token) == true){
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
        else if(varStarted){
            strcat(var, token);
            strcpy(token,"");
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
    
    *returnSize = tokenListSize;
    return tokenList;
}

void parse(char **tokenList, int tokenListSize)
{
    printf("\nPARSE! TOKEN LIST: \n");
    for(int i = 0; i < tokenListSize; i++){
        printf("%s\n", tokenList[i]);
    }
    printf("\n");
    
    int i = 0;
    while(i < tokenListSize){
        
        char prefixForVarToken[4];
        memcpy(prefixForVarToken, tokenList[i], 3);
        prefixForVarToken[3] = '\0';
        
        if(strcmp(tokenList[i],"DISP") == 0){
            /* Get token prefix */
            char strbuff[10000];
            strcpy(strbuff, tokenList[i+1]);
            char **split = str_split(strbuff, ':');
            char *prefix = split[0];
            free(split);
            
            if(strcmp(prefix,"STRING") == 0){
                /* PRINT STRING COMMAND */
                /* Get string as substring of token to print */
                char *literal = new char[MAX_STRING_SIZE];
                char *token = tokenList[i+1];
                int tokenSize = strlen(token);
                memcpy(literal, token+9, (tokenSize-9)+1);
                literal[strlen(literal)-1] = '\0';
                
                /* Print */
                char line[100];
                sprintf(line, "printf(\"%s\\n\");\n", literal);
                fputs(line, oFile);
                delete [] literal;
            }
            else if(strcmp(prefix,"NUM") == 0){
                /* Get string as substring of token to print */
                char *literal = new char[MAX_STRING_SIZE];
                char *token = tokenList[i+1];
                int tokenSize = strlen(token);
                memcpy(literal, token+5, (tokenSize-5));
                literal[(tokenSize-5)] = '\0';
                
                /* Print */
                char line[100];
                sprintf(line, "printf(\"%s\\n\");\n", literal);
                fputs(line, oFile);
                delete [] literal;
            }
            else if(strcmp(prefix,"EXPR") == 0){
                /* Get string as substring of token to print */
                char *literal = new char[MAX_STRING_SIZE];
                char *token = tokenList[i+1];
                int tokenSize = strlen(token);
                memcpy(literal, token+6, (tokenSize-6));
                literal[(tokenSize-6)] = '\0';
                
                /* Evaluate expression */
                evaluateExpressionInC(literal);
                
                /* Print */
                char line[100];
                sprintf(line, "printf(\"%%lu\\n\", expression);\n");
                fputs(line, oFile);
                delete [] literal;
            }
            else if(strcmp(prefix,"VAR") == 0){
                /* Get string as substring of token to print */
                char *varname = new char[MAX_STRING_SIZE];
                char *token = tokenList[i+1];
                int tokenSize = strlen(token);
                memcpy(varname, token+5, (tokenSize-5));
                varname[(tokenSize-5)] = '\0';

                /* Print */
                char *vartype = lookupVariableType(varname+1);
                printf("PRINT VARTYPE: %s\n", vartype);
                if(strcmp(vartype,"STRING") == 0){
                    char line[100];
                    sprintf(line, "printf(\"%%s\\n\", %s);\n", varname+1);
                    fputs(line, oFile);
                }
                else if(strcmp(vartype,"NUM") == 0){
                    char line[100];
                    sprintf(line, "printf(\"%%lu\\n\", %s);\n", varname+1);
                    fputs(line, oFile);
                }
                
                delete [] varname;
            }
            i += 2;
        }
        else if(strcmp(prefixForVarToken,"VAR") == 0 && ((i+2) <= tokenListSize-1) && strcmp(tokenList[i+1],"ASSIGN") == 0){
            /* Get var:@<> token prefix */
            char varToken[100];
            strcpy(varToken, tokenList[i]);
            char **varTokenSplit = str_split(varToken, ':');
            char *varname = varTokenSplit[1];
            varname++; // Move past @ to start of variable name
            free(varTokenSplit);
            
            /* Get next token prefix */
            char strbuff[10000];
            strcpy(strbuff, tokenList[i+2]);
            char **split = str_split(strbuff, ':');
            char *prefix = split[0];
            free(split);
            
            char **split2;
            char *prefix2;
            if((i+3) <= tokenListSize-1){
                char strbuff2[1000];
                strcpy(strbuff2, tokenList[i+3]);
                char **split2 = str_split(strbuff2, ':');
                prefix2 = split2[0];
                free(split2);
                if(strcmp(prefix2, "VAR") == 0){
                    printf("FOUND IT |%s| preceding |%s|\n", prefix2, prefix);
                }
            }
            else{
                printf("PIZZA");
            }
            
            
            if(strcmp(prefix,"STRING") == 0){
                /* Get string as substring of token */
                char *value = new char[MAX_STRING_SIZE];
                char *token = tokenList[i+2];
                int tokenSize = strlen(token);
                memcpy(value, token+9, (tokenSize-9)+1);
                value[strlen(value)-1] = '\0';
                
                /* Print */
                char line[100];
                if(isVariableDefined(varname) == false){
                    sprintf(line, "char *%s = \"%s\"", varname, value);
                }
                else{
                    sprintf(line, "%s = \"%s\"", varname, value);
                }
                fputs(line, oFile);
                delete [] value;
                
                /* Store with corresponding type in symbols table */
                symbols_type_table[varname] = "STRING";
            }
            else if(strcmp(prefix,"NUM") == 0){
                /* Get number as substring of token */
                char *value = new char[MAX_DIGIT_SIZE];
                char *token = tokenList[i+2];
                int tokenSize = strlen(token);
                memcpy(value, token+5, (tokenSize-5));
                value[(tokenSize-5)] = '\0';
                
                /* Print */
                char line[100];
                if(isVariableDefined(varname) == false){
                    sprintf(line, "long %s = %s", varname, value);
                }
                else{
                    sprintf(line, "%s = %s", varname, value);
                }
                fputs(line, oFile);
                delete [] value;
                
                /* Store with corresponding type in symbols table */
                symbols_type_table[varname] = "NUM";
            }
            else if(strcmp(prefix,"EXPR") == 0){
                /* Get string as substring of token */
                char *value = new char[MAX_STRING_SIZE];
                char *token = tokenList[i+2];
                int tokenSize = strlen(token);
                memcpy(value, token+6, (tokenSize-6));
                value[(tokenSize-6)] = '\0';
                
                /* Print */
                char line[100];
                if(isVariableDefined(varname) == false){
                    sprintf(line, "long %s = %s", varname, value);
                }
                else{
                    sprintf(line, "%s = %s", varname, value);
                }
                fputs(line, oFile);
                delete [] value;
                
                /* Store with corresponding type in symbols table */
                symbols_type_table[varname] = "NUM";
            }
            else if(strcmp(prefix,"VAR") == 0){
                /* Get string as substring of token */
                char *value = new char[MAX_DIGIT_SIZE];
                char *token = tokenList[i+2];
                int tokenSize = strlen(token);
                memcpy(value, token+5, (tokenSize-5));
                value[(tokenSize-5)] = '\0';
                
                /* Print */
                char line[100];
                char *declarationType;
                if(strcmp(lookupVariableType(value+1), "STRING") == 0){
                    declarationType = "char*";
                }
                else if(strcmp(lookupVariableType(value+1), "NUM") == 0){
                    declarationType = "long";
                }
                if(isVariableDefined(varname) == false){
                    sprintf(line, "%s %s = %s", declarationType, varname, value+1);
                }
                else{
                    sprintf(line, "%s = %s", varname, value);
                }
                fputs(line, oFile);
                delete [] value;
                
                /* Store with corresponding type in symbols table */
                symbols_type_table[varname] = "NUM";
            }
            
            /* For assignments of the form <var> = <expr/num/var> + <var> */
    
            if(strcmp(prefix2,"VAR") == 0 && strcmp(prefix,"EXPR") == 0){
                /* Get varname as substring of token */
                char *varname = new char[MAX_DIGIT_SIZE];
                char *token = tokenList[i+3];
                int tokenSize = strlen(token);
                memcpy(varname, token+5, (tokenSize-5));
                varname[(tokenSize-5)] = '\0';
                
                /* Write to file */
                char line[100];
                sprintf(line, "%s", varname+1);
                fputs(line, oFile);
                
                delete [] varname;
                i++;
                printf("HHHHHH");
            }
            
            fputs(";\n", oFile);
            
            i += 3;
        }
        else if((strcmp(tokenList[i],"IF") == 0 || strcmp(tokenList[i],"ELSEIF") == 0) && strcmp(tokenList[i+4],"THEN") == 0){
            
            if(strcmp(tokenList[i],"ELSEIF") == 0){
                fputs("} ", oFile);
            }
            
            /* Left hand side. Get token prefix */
            char strbuff[100];
            strcpy(strbuff, tokenList[i+1]);
            char **lhsTokenSplit = str_split(strbuff, ':');
            char *lhsTokenPrefix = lhsTokenSplit[0];
            free(lhsTokenSplit);
            //printf("LHS: %s\n", lhsTokenPrefix);
           
            /* Right hand side. Get token prefix */
            char strbuff2[100];
            strcpy(strbuff2, tokenList[i+3]);
            char **rhsTokenSplit = str_split(strbuff2, ':');
            char *rhsTokenPrefix = rhsTokenSplit[0];
            free(rhsTokenSplit);
            //printf("RHS: %s\n", rhsTokenPrefix);
            
            /* Create new if-scope */
            fputs("if(", oFile);
            
            /** Create conditional expression based on type of lhs and rhs **/
            /* Get left value of expression */
            char *lhsValue;
            char *rhsValue;
            if(strcmp(lhsTokenPrefix,"NUM") == 0){
                lhsValue = new char[MAX_DIGIT_SIZE];
                char *lhsToken = tokenList[i+1];
                int lhsTokenSize = strlen(lhsToken);
                memcpy(lhsValue, lhsToken+5, (lhsTokenSize-5));
                lhsValue[(lhsTokenSize-5)] = '\0';
            }
            else if(strcmp(lhsTokenPrefix,"VAR") == 0){
                lhsValue = new char[MAX_DIGIT_SIZE];
                char *lhsToken = tokenList[i+1];
                int lhsTokenSize = strlen(lhsToken);
                memcpy(lhsValue, lhsToken+5, (lhsTokenSize-5));
                lhsValue[(lhsTokenSize-5)] = '\0';
            }
            
            /* Get right value of expression */
            if(strcmp(rhsTokenPrefix,"NUM") == 0){
                rhsValue = new char[MAX_DIGIT_SIZE];
                char *rhsToken = tokenList[i+3];
                int rhsTokenSize = strlen(rhsToken);
                memcpy(rhsValue, rhsToken+5, (rhsTokenSize-5));
                rhsValue[(rhsTokenSize-5)] = '\0';
            }
            else if(strcmp(rhsTokenPrefix,"VAR") == 0){
                rhsValue = new char[MAX_DIGIT_SIZE];
                char *rhsToken = tokenList[i+3];
                int rhsTokenSize = strlen(rhsToken);
                memcpy(rhsValue, rhsToken+5, (rhsTokenSize-5));
                rhsValue[(rhsTokenSize-5)] = '\0';
            }
            
            //printf("LV: %s\n", lhsValue);
            //printf("RV: %s\n", rhsValue+1);
            
            /* Create expression based on operator */
            char condition[100];
            if(strcmp(tokenList[i+2],"EQUALITY") == 0){
                sprintf(condition, "%s == %s", lhsValue, rhsValue+1);
            }
            else if(strcmp(tokenList[i+2],"LESSOREQUALS") == 0){
                sprintf(condition, "%s <= %s", lhsValue, rhsValue+1);
            }
            else if(strcmp(tokenList[i+2],"GREATEROREQUALS") == 0){
                sprintf(condition, "%s >= %s", lhsValue, rhsValue+1);
            }
            else if(strcmp(tokenList[i+2],"NOTEQUALS") == 0){
                sprintf(condition, "%s != %s", lhsValue, rhsValue+1);
            }
            else if(strcmp(tokenList[i+2],"GREATER") == 0){
                sprintf(condition, "%s > %s", lhsValue, rhsValue+1);
            }
            else if(strcmp(tokenList[i+2],"LESS") == 0){
                sprintf(condition, "%s < %s", lhsValue, rhsValue+1);
            }
            fputs(condition, oFile);
            
            fputs("){\n", oFile);
            i += 5;
        }
        else if(strcmp(tokenList[i],"ELSE") == 0){
            /* Create else-scope */
            fputs("} ", oFile);
            fputs("else{\n", oFile);
            i++;
        }
        else if(strcmp(tokenList[i],"ENDIF") == 0){
            fputs("}\n", oFile);
            i++;
        }
        else if((strcmp(tokenList[i],"WHILE") == 0) && strcmp(tokenList[i+4],"THEN") == 0){
            
            if(strcmp(tokenList[i],"ELSEIF") == 0){
                fputs("} ", oFile);
            }
            
            /* Left hand side. Get token prefix */
            char strbuff[100];
            strcpy(strbuff, tokenList[i+1]);
            char **lhsTokenSplit = str_split(strbuff, ':');
            char *lhsTokenPrefix = lhsTokenSplit[0];
            free(lhsTokenSplit);
            printf("LHS: %s\n", lhsTokenPrefix);
            
            /* Right hand side. Get token prefix */
            char strbuff2[100];
            strcpy(strbuff2, tokenList[i+3]);
            char **rhsTokenSplit = str_split(strbuff2, ':');
            char *rhsTokenPrefix = rhsTokenSplit[0];
            free(rhsTokenSplit);
            printf("RHS: %s\n", rhsTokenPrefix);
            
            /* Create new if-scope */
            fputs("while(", oFile);
            
            /** Create conditional expression based on type of lhs and rhs **/
            /* Get left value of expression */
            char *lhsValue;
            char *rhsValue;
            if(strcmp(lhsTokenPrefix,"NUM") == 0){
                lhsValue = new char[MAX_DIGIT_SIZE];
                char *lhsToken = tokenList[i+1];
                int lhsTokenSize = strlen(lhsToken);
                memcpy(lhsValue, lhsToken+5, (lhsTokenSize-5));
                lhsValue[(lhsTokenSize-5)] = '\0';
            }
            else if(strcmp(lhsTokenPrefix,"VAR") == 0){
                lhsValue = new char[MAX_DIGIT_SIZE];
                char *lhsToken = tokenList[i+1];
                int lhsTokenSize = strlen(lhsToken);
                memcpy(lhsValue, lhsToken+5, (lhsTokenSize-5));
                lhsValue[(lhsTokenSize-5)] = '\0';
                lhsValue++;
            }
            
            /* Get right value of expression */
            if(strcmp(rhsTokenPrefix,"NUM") == 0){
                rhsValue = new char[MAX_DIGIT_SIZE];
                char *rhsToken = tokenList[i+3];
                int rhsTokenSize = strlen(rhsToken);
                memcpy(rhsValue, rhsToken+5, (rhsTokenSize-5));
                rhsValue[(rhsTokenSize-5)] = '\0';
            }
            else if(strcmp(rhsTokenPrefix,"VAR") == 0){
                rhsValue = new char[MAX_DIGIT_SIZE];
                char *rhsToken = tokenList[i+3];
                int rhsTokenSize = strlen(rhsToken);
                memcpy(rhsValue, rhsToken+5, (rhsTokenSize-5));
                rhsValue[(rhsTokenSize-5)] = '\0';
            }
            
            printf("LV: %s\n", lhsValue);
            printf("RV: %s\n", rhsValue+1);
            
            /* Create expression based on operator */
            char condition[100];
            if(strcmp(tokenList[i+2],"EQUALITY") == 0){
                sprintf(condition, "%s == %s", lhsValue, rhsValue+1);
            }
            else if(strcmp(tokenList[i+2],"LESSOREQUALS") == 0){
                sprintf(condition, "%s <= %s", lhsValue, rhsValue+1);
            }
            else if(strcmp(tokenList[i+2],"GREATEROREQUALS") == 0){
                sprintf(condition, "%s >= %s", lhsValue, rhsValue+1);
            }
            else if(strcmp(tokenList[i+2],"NOTEQUALS") == 0){
                sprintf(condition, "%s != %s", lhsValue, rhsValue+1);
            }
            else if(strcmp(tokenList[i+2],"GREATER") == 0){
                sprintf(condition, "%s > %s", lhsValue, rhsValue+1);
            }
            else if(strcmp(tokenList[i+2],"LESS") == 0){
                sprintf(condition, "%s < %s", lhsValue, rhsValue+1);
            }
            fputs(condition, oFile);
            
            fputs("){\n", oFile);
            i += 5;
        }
        else if(strcmp(tokenList[i],"ENDWHILE") == 0){
            fputs("}\n", oFile);
            i++;
        }
        else if(strcmp(tokenList[i],"INCREMENT") == 0){
            /* Get prefix */
            char *nextTokenPrefix = new char[MAX_DIGIT_SIZE];
            char *nextToken = tokenList[i+1];
            int nextTokenSize = strlen(nextToken);
            memcpy(nextTokenPrefix, nextToken, 3);
            nextTokenPrefix[3] = '\0';
            
            if(strcmp(nextTokenPrefix,"VAR") == 0){
                /* Get string as substring of token */
                char *varname = new char[MAX_DIGIT_SIZE];
                char *token = tokenList[i+1];
                int tokenSize = strlen(token);
                memcpy(varname, token+5, (tokenSize-5));
                varname[(tokenSize-5)] = '\0';
                
                /* Increment value */
                char line[100];
                sprintf(line, "%s++;\n", varname+1);
                fputs(line, oFile);
            }

            
            i += 2;
        }
        else{
            i++;
        }
    }
}

int main(int argc, const char * argv[]) {

    openOutputFile();
    
    /* Loop through each command line argument */
    for (int nArg=1; nArg < argc; nArg++){
        char *filecontents = readFile(argv[nArg]);
        if(filecontents != NULL){
            int tokenListSize= 0;
            char **tokenList = lex(filecontents, &tokenListSize);
            parse(tokenList, tokenListSize);
        }
    }
    
    closeOutputFile();
    system("g++ language_run.cpp -o gary_executable");
    system("./gary_executable");

    return 0;
}
