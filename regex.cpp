#include"nfa.h"
#include"regex.h"
#include<iostream>
#include<string>
#include<stack>
#include<vector>
using namespace std;

Regex::Regex(string patstr,int mode,bool minimize){
    this->pattern_str=patstr;
    this->mode=mode;
    this->minimize=minimize;
    this->build();
}

void Regex::build(){
    if(this->mode==1){
        nfa=NFABuilder(pattern_str);
        nfa.build();
    }
}

void Regex::move(set<NFA*>& st,char ch){
    auto out_set=set<NFA*>();
    for(auto it:st){
        if(it->edge==ch||it->type==ANYCHAR||it->type==SETS&&it->st.find(ch)!=it->st.end()){
            out_set.insert(it->link1);
        }
    }
    st=out_set;
}

void Regex::closure_set(set<NFA*>& st){
    if(st.size()==0){
        return;
    }
    stack<NFA*> stk;
    for(auto it : st){
        stk.push(it);
    }
    while(!stk.empty()){
        auto nfa=stk.top();
        stk.pop();
        if(nfa->link1&&nfa->type==EMPTY){
            if(st.find(nfa->link1)==st.end()){
                st.insert(nfa->link1);
                stk.push(nfa->link1);
            }
        }
        if(nfa->link2&&nfa->type==EMPTY){
            if(st.find(nfa->link2)==st.end()){
                st.insert(nfa->link2);
                stk.push(nfa->link2);
            }
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
        return false;
    }
    return false;
}

int main(){
    vector<Sample> samples={
        {"THI","([A-Z]*|[0-9]+)",true},
        {"THISISREGEXTEST","([A-Z]+[0-9]+)",false},
        {"234234abcdefg[*+","([A-Z]+[0-9]*abcdefg)(\\[\\*\\+)",false},
        {"AS342abcdefg234aaaaabccccczczxczcasdzxc","([A-Z]+[0-9]*abcdefg)([0-9]*)(\\*?|a+)(zx|bc*)([a-z]+|[0-9]*)(asd|fgh)(zxc)",true},
        {"abc","[^0-9]*",true}
    };
    for(auto it:samples){
        Regex reg(it.pattern);
        if(reg.match(it.input_str)==it.result){
            log("RIGHT");
        }else{
            log("WRONG");
        }
    }
    return 0;
}