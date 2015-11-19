#include <iostream>
#include "Includes/Utility.h"
#include "Includes/constants.h"
#include "KeywordManager.h"




int main(int argc, const char * argv[]){
    
    /* Setup dictionary */
    initialiseKeywordsTable();
    
    if(argc == 2 && strcmp(argv[1], "-r") == 0){
        /* Set as default */
    }
    else{
        updateKeywordsTableFromCommandLine(argc, argv);
    }
    
    
    /* Write dictionary to file */
    FILE *output = fopen(KEYWORD_CONFIG_FILE_PATH,"w");
    
    typedef std::map<char*, char*>::iterator it_type;
    for(it_type iterator = custom_keywords_table.begin(); iterator != custom_keywords_table.end(); iterator++) {
        fprintf(output, "%s%%%s\n", iterator->first, iterator->second);
    }
    
    /* Close file */
    fclose(output);

}