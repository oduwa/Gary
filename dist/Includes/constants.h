#ifndef __Constants__
#define __Constants__

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define KEYWORD_CONFIG_FILE_PATH "Includes/keywords.kw"
#define KEYWORD_COUNT 19

static int MAX_LANG_FILE_SIZE = 5000000;
static int MAX_STRING_SIZE = 100000;
static int MAX_DIGIT_SIZE = 10;
static int MAX_IDENTIFIER_SIZE = 100;
static int MAX_TOKEN_LIST_SIZE = 100000;
static int MAX_EXPRESSION_SIZE = 200;
static int MAX_OPERATOR_SIZE = 20;

#endif