#include"regex.h"
#include<iostream>
#include<string>
#include<stack>
#include<vector>
#include<set>
#include<map>
#include<time.h>
#include<stdio.h>
using namespace std;

Regex::Regex(string patstr,int mode,bool minimize){
    this->pattern_str=patstr;
    this->mode=mode;
    this->minimize=minimize;
    this->build();
}

void Regex::build(){
    DFA::counter=0;
    NFA::counter=0;
    DFAGroup::counter=0;
    if(this->mode==1){
        nfa=NFABuilder(pattern_str);
        nfa.build();
    }else if(this->mode==2){
        dfa=DFABuilder(pattern_str,minimize);
        dfa.build();
        if(minimize){
            dfa.minimize_dfa(dfa.table);
        }
    }
}

bool Regex::accepted(set<NFA*>& nfa_st){
    for(auto it:nfa_st){
        if(!it->link1&&!it->link2){
            return true;
        }
    }
    return false;
}

bool Regex::match(string inpstr){
    if(mode==1){
        auto cur_set=set<NFA*>();
        cur_set.insert(nfa.nfa_pair.first);
        closure_set(cur_set);
        set<NFA*> next_set=cur_set;
        for(int i=0;i<inpstr.size();i++){
            move(next_set,inpstr[i]);
             cur_set=next_set;
             closure_set(next_set);
             if(next_set.size()==0){
                return false;
             }
             if(accepted(next_set)&&i==inpstr.size()-1){
                return true;
             }
        }
    }else if(mode==2){
        return dfa.dfa_match(inpstr);
    }
    return false;
}