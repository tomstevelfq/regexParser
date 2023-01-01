#include"global.h"
#include<string>
#include<iostream>
#include<set>
#include<stack>
using namespace std;
map<char,Token> TokenMap{
    {'.',ANY},
    {'^',AT_BOL},
    {'$',AT_EOL},
    {']',CCL_END},
    {'[',CCL_START},
    {'}',CLOSE_CURLY},
    {')',CLOSE_PAREN},
    {'*',CLOSURE},
    {'-',DASH},
    {'{',OPEN_CURLY},
    {'(',OPEN_PAREN},
    {'?',OPTIONAL},
    {'|',OR},
    {'+',PLUS_CLOSE}
};
void log(string s){
    cout<<s<<endl;
}