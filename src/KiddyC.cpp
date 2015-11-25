#include <stdio.h>
#include <iostream>
#include <cstring>
#include <cstdlib>

#include "Includes/constants.h"
#include "Includes/Globals.h"
#include "Includes/Utility.h"

#include "Lexer.h"
#include "Parser.h"
#include "KeywordManager.h"

using namespace std;

void openOutputFile()
{
    main_output = fopen("language_run.cpp","w");
    
    fputs("#include <stdio.h>\n#include <iostream>\n#include <cstring>\n#include <cstdlib>\n#include <vector>\n#include \"Includes/constants.h\"\n#include \"Includes/BuiltInFunctions.h\"\n#include \"interpreted_functions.h\"\n\nint main(){\n\n", main_output);
    fputs("printf(\"%sGARY:\\n\", KMAG);\n\n", main_output);
    fputs("long expression;\n", main_output);
    
    
    functions_output = fopen("interpreted_functions.h","w");
    fputs("#include <stdio.h>\n#include <iostream>\n#include \"Includes/constants.h\"\nusing namespace std;\n\n", functions_output);
}

void closeOutputFile()
{
    fputs("printf(\"%s\\n\", KNRM);\n\n", main_output);
    fputs("\n}", main_output);
    fclose(main_output);
    
    fclose(functions_output);
}

int main(int argc, const char * argv[]) {
    
    openOutputFile();
    
    loadKeywordsFromConfigFile();
    
    /* Loop through each command line argument */
    for (int i=1; i < argc; i++){

        if(strcmp(argv[i], "-T") == 0){
            isPrintingEnabled = true;
        }
        
        if(strcmp(argv[i], "-F") == 0){
            showGeneratedFiles = true;
        }
        
        /* If last argument */
        if(i == argc-1){
            /* Append new line to end of source file */
            appendToFile(argv[i], "\n");
            
            /* Get source */
            char *filecontents = readFile(argv[i]);
            if(filecontents != NULL){
                int tokenListSize= 0;
                char **tokenList = lex(filecontents, &tokenListSize);
                parse(tokenList, tokenListSize);
            }
        }
    }
    
    
    if(isPrintingEnabled){
        /* Print variable symbols table */
        printf("%s\n\nSYMBOLS TABLE\n%s", "\x1B[33m", "\x1B[0m");
        typedef std::map<char*, char*>::iterator it_type;
        for(it_type iterator = symbols_type_table.begin(); iterator != symbols_type_table.end(); iterator++) {
            printf("%s %s -> %s\n%s", "\x1B[33m", iterator->first, iterator->second, "\x1B[0m");
        }
        
        printf("%s\n\nFUNCTIONS TABLE\n%s", "\x1B[33m", "\x1B[0m");
        typedef std::map<char*, char*>::iterator it_type;
        for(it_type iterator = function_symbol_table.begin(); iterator != function_symbol_table.end(); iterator++) {
            printf("%s %s -> %s\n%s", "\x1B[33m", iterator->first, iterator->second, "\x1B[0m");
        }
        
        printf("%s\nKEYWORDS TABLE\n%s", "\x1B[33m", "\x1B[0m");
        typedef std::map<char*, char*>::iterator it_type;
        for(it_type iterator = custom_keywords_table.begin(); iterator != custom_keywords_table.end(); iterator++) {
            printf("%s %s -> %s\n%s", "\x1B[33m", iterator->first, iterator->second, "\x1B[0m");
        }
    }

    closeOutputFile();
    
    /* Detect command line environment */
    #ifdef _WIN32
    
    int res = system("g++ language_run.cpp -o language_executable");
    if(res == 0){
        system("language_executable.exe");
        
        if(!showGeneratedFiles){
            system("del language_run.cpp;del language_executable.exe;del interpreted_functions.h");
        }
    }
    else{
        printf("COMPILE ERROR\n");
    }
    
    #else
    
    int res = system("g++ language_run.cpp -o language_executable");
    if(res == 0){
        system("./language_executable");
        
        if(!showGeneratedFiles){
            system("rm language_run.cpp;rm language_executable;rm interpreted_functions.h");
        }
    }
    else{
        printf("COMPILE ERROR\n");
    }
    
    #endif

    

    return 0;
}
