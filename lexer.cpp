#include "lexer.h"
#include<iostream>
using namespace std;


Lexer::Lexer(string pat):pattern(pat){}

Token Lexer::getToken(char letter){
    auto it=TokenMap.find(letter);
    if(it!=TokenMap.end()){
        return it->second;
    }else{
        return Token::L;
    }
}

Token Lexer::advance(){
    if(pos>=pattern.size()){
        this->cur_token=EOS;
        return EOS;
    }
    letter=pattern[pos];
    if(letter=='\\'){
        isescape=!isescape;
        pos++;
        cur_token=escape();
    }else{
        cur_token=analysis(letter);
    }
    return cur_token;
}

Token Lexer::escape(){
    letter=pattern[pos];
    pos++;
    return Token::L;
}

Token Lexer::analysis(char letter){
    pos++;
    return getToken(letter);
}

bool Lexer::match(Token tk){
    return this->cur_token==tk;
}