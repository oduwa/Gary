#ifndef __Parser__
#define __Parser__

#include <iostream>
#include <cstring>
#include <cstdlib>
#include "Includes/constants.h"
#include "Includes/Globals.h"
#include "Includes/Utility.h"
#include "KeywordManager.h"

using namespace std;


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
    
    return "";
}

char* lookupFunctionType(char* varname)
{
    typedef std::map<char*, char*>::iterator it_type;
    for(it_type iterator = function_symbol_table.begin(); iterator != function_symbol_table.end(); iterator++) {
        // iterator->first = key
        // iterator->second = value
        if(strcmp(iterator->first, varname) == 0){
            return iterator->second;
        }
    }
    
    return "";
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

void evaluateExpressionInC(const char *expr)
{
    char line[100];
    sprintf(line, "expression = %s;\n", expr);
    fputs(line, main_output);
}

void parse(char **tokenList, int tokenListSize)
{
    if(isPrintingEnabled){
        printf("%s\nPARSE! TOKEN LIST: \n", KGRN);
        for(int i = 0; i < tokenListSize; i++){
            printf("%s\n", tokenList[i]);
        }
        printf("\nPARSE! END TOKEN LIST: \n%s", KNRM);
        printf("\n");
    }
    
    bool isWritingFunction = false;
    
    int i = 0;
    while(i < tokenListSize){
        
        char prefixForVarToken[4];
        memcpy(prefixForVarToken, tokenList[i], 3);
        prefixForVarToken[3] = '\0';
        
        if(strcmp(tokenList[i],"DISP") == 0){
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
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
                fputs(line, fileOutput);
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
                fputs(line, fileOutput);
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
                sprintf(line, "printf(\"%%ld\\n\", expression);\n");
                fputs(line, fileOutput);
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
                //printf("PRINT VARTYPE: %s\n", vartype);
                if(strcmp(vartype,"STRING") == 0){
                    char line[100];
                    //sprintf(line, "printf(\"%%s\\n\", %s);\n", varname+1);
                    sprintf(line, "std::cout << %s << std::endl;\n", varname+1);
                    fputs(line, fileOutput);
                }
                else if(strcmp(vartype,"NUM") == 0){
                    char line[100];
                    sprintf(line, "printf(\"%%ld\\n\", %s);\n", varname+1);
                    fputs(line, fileOutput);
                }
                else if(strstr(vartype, "LIST<") != NULL){
                    char line[100];
                    sprintf(line, "printVector(%s);\n", varname+1);
                    fputs(line, fileOutput);
                }
                
                delete [] varname;
            }
            i += 2;
        }
        else if(strcmp(prefixForVarToken,"VAR") == 0 && ((i+2) <= tokenListSize-1) && strcmp(tokenList[i+1],"ASSIGN") == 0){
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
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
                fputs(line, fileOutput);
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
                fputs(line, fileOutput);
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
                fputs(line, fileOutput);
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
                fputs(line, fileOutput);
                delete [] value;
                
                /* Store with corresponding type in symbols table */
                symbols_type_table[varname] = "NUM";
            }
            else if(strcmp(tokenList[i+2], "MKLST") == 0){
                /* Is trying to create a new array. Get array properties */
                char *callToken = tokenList[i+3];
                char *callParameters = new char[MAX_DIGIT_SIZE+10];
                strcpy(callParameters, callToken+6);
                callParameters[strlen(callParameters)-1] = '\0';
                
                /* Split array parameters to get them separately from "<type>, <size>" to
                 * "<type>" and "<size>"
                 */
                char **paramsSplit = str_split(callParameters, ',');
                char *listType = paramsSplit[0];
                char *sizeParam = paramsSplit[1];
                free(paramsSplit);
                int listSize = atoi(sizeParam);
                
                /* Write to file */
                if(isVariableDefined(varname) == false){
                    fprintf(fileOutput, "std::vector<%s> %s = std::vector<%s>(%d);\n", listType, varname, listType, listSize);
                }
                else{
                    fprintf(fileOutput, "%s = std::vector<%s>(%d);\n", varname, listType, listSize);
                }
                
                /* Increment i by 1 so language doesnt try to call the CALL token */
                i++;
                
                /* Store with corresponding type in symbols table */
                char *tableEntry = new char[100];
                sprintf(tableEntry, "LIST<%s>", listType);
                symbols_type_table[varname] = tableEntry;
                
                /* Cleanup */
                delete [] callParameters;
                free(listType);
                free(sizeParam);
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
                fputs(line, fileOutput);
                
                delete [] varname;
                i++;
                
                /* Store with corresponding type in symbols table */
                symbols_type_table[varname] = "NUM";
            }
            
            fputs(";\n", fileOutput);
            
            i += 3;
        }
        else if((strcmp(tokenList[i],"IF") == 0 || strcmp(tokenList[i],"ELSEIF") == 0) && strcmp(tokenList[i+4],"THEN") == 0){
            
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
            if(strcmp(tokenList[i],"ELSEIF") == 0){
                fputs("} ", fileOutput);
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
            fputs("if(", fileOutput);
            
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
            else if(strcmp(lhsTokenPrefix,"STRING") == 0){
                lhsValue = new char[MAX_DIGIT_SIZE];
                char *lhsToken = tokenList[i+1];
                int lhsTokenSize = strlen(lhsToken);
                memcpy(lhsValue, lhsToken+8, (lhsTokenSize-8));
                lhsValue[(lhsTokenSize-8)] = '\0';
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
            else if(strcmp(rhsTokenPrefix,"STRING") == 0){
                rhsValue = new char[MAX_DIGIT_SIZE];
                char *rhsToken = tokenList[i+3];
                int rhsTokenSize = strlen(rhsToken);
                memcpy(rhsValue, rhsToken+8, (rhsTokenSize-8));
                rhsValue[(rhsTokenSize-8)] = '\0';
            }
            
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
            
            /* Handle String comparisons */
            if(strcmp(lhsTokenPrefix,"STRING") == 0 || strcmp(rhsTokenPrefix,"STRING") == 0){
                if(strcmp(tokenList[i+2],"EQUALITY") == 0){
                    sprintf(condition, "strcmp(%s, %s) == 0", lhsValue, rhsValue+1);
                }
                else if(strcmp(tokenList[i+2],"LESSOREQUALS") == 0){
                    sprintf(condition, "strcmp(%s, %s) <= 0", lhsValue, rhsValue+1);
                }
                else if(strcmp(tokenList[i+2],"GREATEROREQUALS") == 0){
                    sprintf(condition, "strcmp(%s, %s) >= 0", lhsValue, rhsValue+1);
                }
                else if(strcmp(tokenList[i+2],"NOTEQUALS") == 0){
                    sprintf(condition, "strcmp(%s, %s) != 0", lhsValue, rhsValue+1);
                }
                else if(strcmp(tokenList[i+2],"GREATER") == 0){
                    sprintf(condition, "strcmp(%s, %s) > 0", lhsValue, rhsValue+1);
                }
                else if(strcmp(tokenList[i+2],"LESS") == 0){
                    sprintf(condition, "strcmp(%s, %s) < 0", lhsValue, rhsValue+1);
                }
            }

            
            fputs(condition, fileOutput);
            
            fputs("){\n", fileOutput);
            i += 5;
        }
        else if((strcmp(tokenList[i],"IF") == 0 || strcmp(tokenList[i],"ELSEIF") == 0) && (strcmp(tokenList[i+4],"&&") == 0 || strcmp(tokenList[i+4],"||") == 0)){
            
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
            if(strcmp(tokenList[i],"ELSEIF") == 0){
                fputs("} ", fileOutput);
            }
            
            /* Create new if-scope */
            fputs("if(", fileOutput);
            
            /* Loop to keep adding conditions for every time theres an "&&" or "||" */
            int conditionCount = 0;
            char conditions[100];
            while(strcmp(tokenList[i],"IF") == 0 || strcmp(tokenList[i],"ELSEIF") == 0 || strcmp(tokenList[i],"&&") == 0 || strcmp(tokenList[i],"||") == 0){
                /* Left hand side. Get token prefix */
                char strbuff[100];
                strcpy(strbuff, tokenList[i+1]);
                char **lhsTokenSplit = str_split(strbuff, ':');
                char *lhsTokenPrefix = lhsTokenSplit[0];
                free(lhsTokenSplit);
                
                /* Right hand side. Get token prefix */
                char strbuff2[100];
                strcpy(strbuff2, tokenList[i+3]);
                char **rhsTokenSplit = str_split(strbuff2, ':');
                char *rhsTokenPrefix = rhsTokenSplit[0];
                free(rhsTokenSplit);
                
                
                
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
                else if(strcmp(lhsTokenPrefix,"STRING") == 0){
                    lhsValue = new char[MAX_DIGIT_SIZE];
                    char *lhsToken = tokenList[i+1];
                    int lhsTokenSize = strlen(lhsToken);
                    memcpy(lhsValue, lhsToken+8, (lhsTokenSize-8));
                    lhsValue[(lhsTokenSize-8)] = '\0';
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
                else if(strcmp(rhsTokenPrefix,"STRING") == 0){
                    rhsValue = new char[MAX_DIGIT_SIZE];
                    char *rhsToken = tokenList[i+3];
                    int rhsTokenSize = strlen(rhsToken);
                    memcpy(rhsValue, rhsToken+8, (rhsTokenSize-8));
                    rhsValue[(rhsTokenSize-8)] = '\0';
                }
                
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
                
                /* Handle String comparisons */
                if(strcmp(lhsTokenPrefix,"STRING") == 0 || strcmp(rhsTokenPrefix,"STRING") == 0){
                    if(strcmp(tokenList[i+2],"EQUALITY") == 0){
                        sprintf(condition, "strcmp(%s, %s) == 0", lhsValue, rhsValue+1);
                    }
                    else if(strcmp(tokenList[i+2],"LESSOREQUALS") == 0){
                        sprintf(condition, "strcmp(%s, %s) <= 0", lhsValue, rhsValue+1);
                    }
                    else if(strcmp(tokenList[i+2],"GREATEROREQUALS") == 0){
                        sprintf(condition, "strcmp(%s, %s) >= 0", lhsValue, rhsValue+1);
                    }
                    else if(strcmp(tokenList[i+2],"NOTEQUALS") == 0){
                        sprintf(condition, "strcmp(%s, %s) != 0", lhsValue, rhsValue+1);
                    }
                    else if(strcmp(tokenList[i+2],"GREATER") == 0){
                        sprintf(condition, "strcmp(%s, %s) > 0", lhsValue, rhsValue+1);
                    }
                    else if(strcmp(tokenList[i+2],"LESS") == 0){
                        sprintf(condition, "strcmp(%s, %s) < 0", lhsValue, rhsValue+1);
                    }
                }
                
                
                
                if(conditionCount > 0){
                    if(strcmp(tokenList[i],"&&") == 0){
                        strcat(conditions, " && ");
                    }
                    else if(strcmp(tokenList[i],"||") == 0){
                        strcat(conditions, " || ");
                    }
                }
                strcat(conditions, condition);
                
                conditionCount++;
                i += 4;
            }
            if(isPrintingEnabled){
                printf("CONDITIONS: %s\n", conditions);
            }

            fputs(conditions, fileOutput);
            
            fputs("){\n", fileOutput);
            i++;
        }
        else if(strcmp(tokenList[i],"ELSE") == 0){
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
            /* Create else-scope */
            fputs("} ", fileOutput);
            fputs("else{\n", fileOutput);
            i++;
        }
        else if(strcmp(tokenList[i],"ENDIF") == 0){
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
            fputs("}\n", fileOutput);
            i++;
        }
        else if((strcmp(tokenList[i],"WHILE") == 0) && strcmp(tokenList[i+4],"THEN") == 0){
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
            if(strcmp(tokenList[i],"ELSEIF") == 0){
                fputs("} ", fileOutput);
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
            fputs("while(", fileOutput);
            
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
            
            fputs(condition, fileOutput);
            
            fputs("){\n", fileOutput);
            i += 5;
        }
        else if(strcmp(tokenList[i],"ENDWHILE") == 0){
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
            fputs("}\n", fileOutput);
            i++;
        }
        else if(strcmp(tokenList[i],"INCREMENT") == 0){
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
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
                fputs(line, fileOutput);
            }
            
            delete [] nextTokenPrefix;
            
            i += 2;
        }
        else if(strcmp(tokenList[i],"FUNCTION_DECL") == 0 && (i+3) <= tokenListSize-1){
            isWritingFunction = true;
            
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
            /* Get function name from token list */
            char *funcName = tokenList[i+1];
            bool hasArguments = true;
            
            /* Check if function has arguments */
            int j = i+2;
            char *nextToken = tokenList[j];
            int nextTokenSize = strlen(nextToken);
            char *arglist = new char[1000];
            
            /* if doesnt have arguments, token will be "ARG=:" rather than "ARG=<arg1>: ... :<arg2>" */
            if(strlen(nextToken) > 5){
                //printf("FUNC WITH ARGS\n");
                char nextTokenPrefix[4];
                memcpy(nextTokenPrefix, nextToken, 3);
                nextTokenPrefix[3] = '\0';
                
                while(strcmp(nextTokenPrefix,"ARG") == 0){
                    /* Append arguments to argument list */
                    char **split = str_split(nextToken+4, ':');
                    strcat(arglist, split[0]);
                    strcat(arglist, " ");
                    strcat(arglist, split[1]);
                    
                    /* Store with corresponding type in symbols table for variables */
                    if(strcmp(split[0], "string") == 0){
                        symbols_type_table[split[1]] = "STRING";
                    }else{
                        symbols_type_table[split[1]] = "NUM";
                    }
                    
                    /* Get next token */
                    j++;
                    nextToken = tokenList[j];
                    nextTokenSize = strlen(nextToken);
                    memcpy(nextTokenPrefix, nextToken, 3);
                    nextTokenPrefix[3] = '\0';
                    
                    /* Handle the fact that if last argument dont add a comma */
                    if(strcmp(nextTokenPrefix,"ARG") == 0){
                        strcat(arglist, ",");
                    }
                }
            }
            else{
                //printf("FUNC WITHOUT ARGS\n");
                j++;
                hasArguments = false;
            }
            
            /* Get function return type. Token is of the form RET:<type> */
            char strbuff[100];
            strcpy(strbuff, tokenList[j]);
            char **split = str_split(strbuff, ':');
            char *funcReturnType = split[1];
            //printf("\tRETURN TYPE = %s FROM TOKEN:%s\n", funcReturnType, tokenList[j]);
            
            if(strcmp(funcReturnType, "string") == 0){
                delete [] funcReturnType;
                funcReturnType = "char*";
            }
            else if(strncmp(funcReturnType, "list", 4) == 0){
                /* Get list type */
                char listType[50];
                int len = strlen(funcReturnType);
                memcpy(listType, funcReturnType+5, len-5-1);
                
                /* Prepare return type for function */
                sprintf(funcReturnType, "std::vector<%s>", listType);
            }
            else{
                // Leave as is
            }
            
            /* Write output */
            char line[1000];
            if(hasArguments){
                sprintf(line, "%s %s(%s){\n", funcReturnType, funcName, arglist);
            }
            else{
                sprintf(line, "%s %s(){\n", funcReturnType, funcName);
            }
            fputs(line, functions_output);
            
            /* Add to symbols table */
            char *tableEntry = new char[100];
            sprintf(tableEntry, "%s", funcReturnType);
            function_symbol_table[funcName] = tableEntry;
            
            /* Cleanup */
            free(split);
            delete [] arglist;
            delete [] funcReturnType;
            
            i++;
        }
        else if(strcmp(tokenList[i],"FUNCTION_CLOSE") == 0){
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
            fputs("}\n", functions_output);
            isWritingFunction = false;
            
            i++;
        }
        else if(strcmp(prefixForVarToken,"CAL") == 0){
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
            /* Get function name from token. Token is of the form CALL:<function_call> */
            char strbuff[100];
            strcpy(strbuff, tokenList[i]);
            char **split = str_split(strbuff, ':');
            char *funcCall = split[1];
            
            /* Write output */
            char line[1000];
            sprintf(line, "%s;\n", funcCall);
            fputs(line, fileOutput);
            
            free(split);
            i++;
        }
        else if(strcmp(tokenList[i],"RETURN") == 0){
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
            /* Process next token to get its prefix (hence what it represents) */
            char *nextToken = tokenList[i+1];
            char nextTokenPrefix[4];
            memcpy(nextTokenPrefix, nextToken, 3);
            nextTokenPrefix[3] = '\0';
            
            /* Return the value corresponding to the token type */
            if(strcmp(nextTokenPrefix,"NUM") == 0){
                /* Get value to return */
                char *returnValue = new char[100];
                int nextTokenSize = strlen(nextToken);
                memcpy(returnValue, nextToken+5, (nextTokenSize-5));
                returnValue[(nextTokenSize-5)] = '\0';
                
                /* Write output */
                char line[1000];
                sprintf(line, "return %s;\n", returnValue);
                fputs(line, fileOutput);
                
                /* Cleanup */
                delete [] returnValue;
            }
            else if(strcmp(nextTokenPrefix,"EXP") == 0){
                /* Get value to return */
                char *returnValue = new char[100];
                int nextTokenSize = strlen(nextToken);
                memcpy(returnValue, nextToken+6, (nextTokenSize-6));
                returnValue[nextTokenSize-6] = '\0';
                
                /* Write output */
                char line[1000];
                sprintf(line, "return %s;\n", returnValue);
                fputs(line, fileOutput);
                
                /* Cleanup */
                delete [] returnValue;
            }
            else if(strcmp(nextTokenPrefix,"VAR") == 0){
                /* Get value to return */
                char *returnValue = new char[100];
                int nextTokenSize = strlen(nextToken);
                memcpy(returnValue, nextToken+6, (nextTokenSize-6));
                returnValue[(nextTokenSize-6)] = '\0';
                
                /* Write output */
                char line[1000];
                sprintf(line, "return %s;\n", returnValue);
                fputs(line, fileOutput);
                
                /* Cleanup */
                delete [] returnValue;
            }
            
            /* Move to next relevant token in token list */
            i += 2;
        }
        else if(strcmp(tokenList[i], "LSTADD") == 0){
            /** ADD VALUE TO ARRAY **/
            
            /* Prepare correct output */
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
            /* Get call arguments */
            char *callToken = tokenList[i+1];
            char *callParameters = new char[MAX_DIGIT_SIZE+10];
            strcpy(callParameters, callToken+6);
            callParameters[strlen(callParameters)-1] = '\0';
            
            /* Split array parameters to get them separately from "@<array_variable_name>, <value_to_add>" to
             * "@<array_variable_name>" and "<value_to_add>"
             */
            char **paramsSplit = str_split(callParameters, ',');
            char *listName = paramsSplit[0];listName++;
            char *listValue = paramsSplit[1];
            free(paramsSplit);
            
            /* Get list type */
            char *listVariableId = lookupVariableType(listName);
            char *listType = new char[100];
            memcpy(listType, listVariableId+5, strlen(listVariableId)-5-1);
            
            /* Handle variable input */
            if(*listValue == '@'){
                listValue++;
                
                /* Write output */
                char line[1000];
                sprintf(line, "%s.push_back(%s);\n", listName, listValue);
                fputs(line, fileOutput);
            }
            /* Handle literal input */
            else{
                /* Handle pushing to list based on type of list */
                if(strcmp(listType, "int") == 0){
                    /* Get value */
                    int value = atoi(listValue);
                    
                    /* Write output */
                    char line[1000];
                    sprintf(line, "%s.push_back(%d);\n", listName, value);
                    fputs(line, fileOutput);
                }
                else if(strcmp(listType, "string") == 0){
                    /* Get value */
                    char *value = listValue;
                    while(*value == ' '){
                        value++;
                    }
                    
                    /* Write output */
                    char line[1000];
                    sprintf(line, "%s.push_back(%s);\n", listName, value);
                    fputs(line, fileOutput);
                }
            }
            
            
            
            
            /* Cleanup */
            delete [] listType;
            
            
            /* Next */
            i+=2;
        }
        else if(strcmp(tokenList[i], "LSTRMV_IDX") == 0){
            /** REMOVE VALUE FROM ARRAY **/
            
            /* Prepare correct output */
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
            /* Get call arguments */
            char *callToken = tokenList[i+1];
            char *callParameters = new char[MAX_DIGIT_SIZE+10];
            strcpy(callParameters, callToken+6);
            callParameters[strlen(callParameters)-1] = '\0';
            
            /* Split array parameters to get them separately from "@<array_variable_name>, <index_to_remove>" to
             * "@<array_variable_name>" and "<index_to_remove>"
             */
            char **paramsSplit = str_split(callParameters, ',');
            char *listName = paramsSplit[0];listName++;
            char *listIndex = paramsSplit[1];
            free(paramsSplit);
            
            /* Handle variable input */
            if(*listIndex == '@'){
                listIndex++;
            }
            
            /* Get value */
            int index_to_remove = atoi(listIndex);
            
            /* Decrement Index to account for the fact that the generated language is indexed from 1 */
            index_to_remove--;
            
            /* Write output */
            char line[1000];
            sprintf(line, "%s.erase(%s.begin() + %d);\n", listName, listName, index_to_remove);
            fputs(line, fileOutput);
            
            
            /* Cleanup */
            free(paramsSplit[0]);
            
            /* Next */
            i+=2;
        }
        else if(strcmp(tokenList[i], "LSTPUT") == 0){
            /** INSERT VALUE INTO ARRAY AT SPECIFIED POSITION **/
            
            /* Prepare correct output */
            FILE *fileOutput;
            fileOutput = (isWritingFunction == false) ? main_output : functions_output;
            
            /* Get call arguments */
            char *callToken = tokenList[i+1];
            char *callParameters = new char[MAX_DIGIT_SIZE+10];
            strcpy(callParameters, callToken+6);
            callParameters[strlen(callParameters)-1] = '\0';
            if(isPrintingEnabled){
               printf("%sPUT WITH ARGUMENTS: %s%s\n", KRED, callParameters, KNRM);
            }
            
            /*
             * Split array parameters to get them separately from "@<array_variable_name>, <index_to_put>, <value>".
             */
            char **paramsSplit = str_split(callParameters, ',');
            char *listName = paramsSplit[0];listName++;
            char *listIndex = paramsSplit[1];
            char *newValue = paramsSplit[2];
            free(paramsSplit);
            if(isPrintingEnabled){
                printf("%sSPLIT  RESULT: x%sx x%sx x%sx\n%s", KRED, listName, listIndex, newValue, KNRM);
            }
            
            
            /* Clean input. convert index to int */
            int index_to_put = atoi(listIndex);
            
            /* Clean input. remove any leading spaces from strings */
            while(*listName == ' '){
                listName++;
            }
            while(*newValue == ' '){
                newValue++;
            }
            
            /* Handle variable input */
            if(*listIndex == '@'){
                listIndex++;
            }
            if(*newValue == '@'){
                newValue++;
            }
            
            /* Decrement Index to account for the fact that the generated language is indexed from 1 */
            index_to_put--;
            
            /* Write output */
            char line[1000];
            sprintf(line, "%s[%d] = %s;\n", listName, index_to_put, newValue);
            fputs(line, fileOutput);
            
            
            /* Cleanup */
            free(listIndex);
            
            /* Next */
            i+=2;
        }
        else{
            i++;
        }
    }
}

#endif
