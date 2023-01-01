#include"dfa.h"

int DFA::counter=0;

DFA* DFABuilder::nfa_to_dfa(set<NFA*>& st){
    DFA* dfa=new DFA();
    for(auto it:st){
        dfa->nfast.insert(it);
        if(it->link1==NULL&&it->link2==NULL){
            dfa->accepted=true;
        }
    }
    dfa->id_num=dfa->counter++;
    table.push_back(vector<int>(128,-1));
    return dfa;
}

DFABuilder::DFABuilder(string pattern){
    this->pattern=pattern;
    DFA::counter=0;
}

void DFABuilder::build(){
    auto nfa=NFABuilder(pattern);
    nfa.build();
    gen_dfa_table(nfa.nfa_pair.first);
}

DFA* DFABuilder::repeated_dfa(set<NFA*>& closure_st){
    for(auto it:st){
        if(it->nfast==closure_st){
            return it;
        }
    }
    return NULL;
}

void DFABuilder::gen_dfa_table(NFA* start){
    set<NFA*> nfa_closure={start};
    closure_set(nfa_closure);
    DFA* dfa=nfa_to_dfa(nfa_closure);
    st.push_back(dfa);
    int pos=0;
    while(pos<st.size()){
        dfa=st[pos];
        for(int i=0;i<127;i++){
            auto nfa_move=dfa->nfast;
            move(nfa_move,char(i));
            if(!nfa_move.empty()){
                closure_set(nfa_move);
                if(nfa_move.empty()){
                    continue;
                }
                DFA* new_dfa=repeated_dfa(nfa_move);
                int next_state=-1;
                if(!new_dfa){
                    new_dfa=nfa_to_dfa(nfa_move);
                    st.push_back(new_dfa);
                    if(new_dfa->accepted){
                        table[new_dfa->id_num][127]=1;
                    }
                }
                next_state=new_dfa->id_num;
                table[dfa->id_num][i]=next_state;
            }
        }
        pos++;
    }
}

bool DFABuilder::repeated(set<NFA*>& closure_set){
    for(auto& it:st){
        if(it->nfast==closure_set){
            return true;
        }
    }
    return false;
}

bool DFABuilder::dfa_match(string input_str){
    int cur_status=0;
    if(minimize){

    }
    for(int i=0;i<input_str.size();i++){
        int next_status=table[cur_status][input_str[i]];
        if(next_status!=-1){
            cur_status=next_status;
        }else{
            return false;
        }
    }
    return table[cur_status][127]==1;
}

map<set<NFA*>,set<NFA*>> mp;
void closure_set(set<NFA*>& st){
    auto it=mp.find(st);
    if(mp.find(st)!=mp.end()){
        st=it->second;
    }else{
        auto bak=st;
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
        mp.insert({bak,st});
    }
}