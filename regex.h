#ifndef REGEXH
#define REGEXH
#include<iostream>
#include"nfa.h"
#include"dfa.h"
#include"lexer.h"
#include"global.h"
using namespace std;
class Regex{
    public:
        string pattern_str;
        int mode;
        bool minimize;
        NFABuilder nfa;
        DFABuilder dfa;
        Regex(string patstr="",int mode=1,bool minimize=false);
        bool match(string inpstr);
        void build();
        bool accepted(set<NFA*>& nfa_set);
};
class Sample{
    public:
        string input_str;
        string pattern;
        bool result;
};
#endif