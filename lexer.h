#ifndef LEXERH
#define LEXERH
#include<iostream>
#include<map>
#include<string>
#include"global.h"
using namespace std;

extern map<char,Token> TokenMap;

class Lexer{
    public:
        string pattern;
        char letter=' ';
        int pos=0;
        bool isescape=false;
        Token cur_token;
        Lexer(string pat="");
        Token escape();
        Token analysis(char letter);
        Token advance();
        bool match(Token tk);
    private:
        Token getToken(char letter);
};
#endif
