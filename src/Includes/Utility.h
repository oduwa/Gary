#ifndef __Utility__
#define __Utility__

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <assert.h>

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

void appendToFile(const char *filename, const char *text)
{
    FILE *file = fopen(filename, "a");
    fprintf(file, "%s", text);
    fclose(file);
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

void lowercase(char *str)
{
    for(int i = 0; str[i]; i++){
        str[i] = (char) std::tolower(str[i]);
    }
}

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

#endif
