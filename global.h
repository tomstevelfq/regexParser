#ifndef GLOBALH
#define GLOBALH
#include<map>
#include<string>
using namespace std;

enum Token{
    EOS,
    ANY,
    AT_BOL,
    AT_EOL,
    CCL_END,
    CCL_START,
    CLOSE_CURLY,
    CLOSE_PAREN,
    CLOSURE,
    DASH,
    END_OF_INPUT,
    L,
    OPEN_CURLY,
    OPEN_PAREN,
    OPTIONAL,
    OR,
    PLUS_CLOSE
};
enum Category{EMPTY,CHAR,SETS,ANYCHAR};
extern map<char,Token> TokenMap;
void log(string s);
#endif