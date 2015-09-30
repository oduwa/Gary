//
//  main.cpp
//  MyLanguage
//
//  Created by Odie Edo-Osagie on 28/09/2015.
//  Copyright © 2015 Odie Edo-Osagie. All rights reserved.
//

#include <iostream>
#include <assert.h>

using namespace std;

static int MAX_LANG_FILE_SIZE = 5000000;
static int MAX_STRING_SIZE = 100000;
static int MAX_TOKEN_LIST_SIZE = 100000;

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
        str[i] = (char) tolower(str[i]);
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
    
    bool isLookingAtString = false;
    bool isExpression = false;
    
    char **tokenList = new char*[MAX_TOKEN_LIST_SIZE];
    int tokenListSize = 0;
    
    printf("File contains %d characters\n", size);
    for(int i = 0; i < size; i++){
        /* increment token */
        appendChar(token, MAX_LANG_FILE_SIZE, filecontents[i]);
        //printf("TOKEN: %s\n", token);
        
        /* lex. Should reset the token variable each time we find a valid token. */
        if(strcmp(token," ") == 0){
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
                printf("\t\tNUMB: %s\n", expressionLiteral);
                
                /* Create and add token for string literal */
                char strbuff[1000];
                sprintf(strbuff, "NUM: %s", expressionLiteral);
                tokenList[tokenListSize] = new char[MAX_STRING_SIZE];
                strcpy(tokenList[tokenListSize], strbuff);
                tokenListSize++;
                
                strcpy(expressionLiteral,"");
            }
            
            isExpression = false;
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
        
        strcpy(lowercaseToken, token);
        lowercase(lowercaseToken);
    }
    
    /* Cleanup */
    delete [] token;
    delete [] lowercaseToken;
    delete [] stringLiteral;
    delete [] expressionLiteral;
    delete [] number;
    
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
                // TODO: Maybe write to external c source file and execute
                printf("\tPROGRAM OUPUT: %s\n", literal);
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
                // TODO: Maybe write to external c source file and execute
                printf("\tPROGRAM OUPUT: %s\n", literal);
                delete [] literal;
            }
            else if(strcmp(prefix,"EXPR") == 0){
                /* Get string as substring of token to print */
                char *literal = new char[MAX_STRING_SIZE];
                char *token = tokenList[i+1];
                int tokenSize = strlen(token);
                memcpy(literal, token+6, (tokenSize-6));
                literal[(tokenSize-6)] = '\0';
                
                /* Print */
                // TODO: Maybe write to external c source file and execute
                printf("\tPROGRAM OUPUT: %s\n", literal);
                delete [] literal;
            }
            i += 2;
        }
        else{
            i++;
        }
    }
}

int main(int argc, const char * argv[]) {

    /* Loop through each command line argument */
    for (int nArg=1; nArg < argc; nArg++){
        char *filecontents = readFile(argv[nArg]);
        if(filecontents != NULL){
            int tokenListSize= 0;
            char **tokenList = lex(filecontents, &tokenListSize);
            parse(tokenList, tokenListSize);
        }
    }
    
    char x[] = "TUTORIALS POINT";
    printf("\n%s\n", x);
    lowercase(x);
    printf("\n%s\n", x);

    return 0;
}
