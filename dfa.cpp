#include"dfa.h"

int DFA::counter=0;
int DFAGroup::counter=0;

DFAGroup::DFAGroup(vector<vector<int>>& trans_table){
    group_num=counter++;
    trans_table.push_back(vector<int>(128,-1));
}

DFA* DFABuilder::nfa_to_dfa(set<NFA*>& st){
    DFA* dfa=new DFA();
    for(auto it:st){
        dfa->nfast.insert(it);
        if(it->link1==NULL&&it->link2==NULL){
            dfa->accepted=true;
        }
    }
    dfa->id_num=dfa->counter++;
    table.push_back(vector<int>(128,-1));//vector前127个代表ascii字符 第128个代表是否accepted
    return dfa;
}

DFABuilder::DFABuilder(string pattern,bool minimize){
    this->pattern=pattern;
    this->minimize=minimize;
    DFA::counter=0;
    on_partition=true;
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
        cur_status=dfa_in_group(0)->group_num;
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

void DFABuilder::minimize_dfa(vector<vector<int>>& jump_table){
    partition_accepted();
    while(on_partition){
        on_partition=false;
        //partition_on_num(table);
        partition_on_char(table);
    }
    create_mindfa_table(table);
}
void DFABuilder::partition_accepted(){
    vector<DFA*> group_ne;
    vector<DFA*> group_e;
    for(auto dfa:st){
        if(dfa->accepted){
            group_e.push_back(dfa);
        }else{
            group_ne.push_back(dfa);
        }
    }
    if(!group_e.empty()){
        auto dfa_group=new DFAGroup(trans_table);
        dfa_group->group=group_e;
        group_list.push_back(dfa_group);
    }
    if(!group_ne.empty()){
        auto dfa_group=new DFAGroup(trans_table);
        dfa_group->group=group_ne;
        group_list.push_back(dfa_group);
    }
}
void DFABuilder::partition_on_num(vector<vector<int>>& jump_table){
    for(auto iter=group_list.begin();iter!=group_list.end();iter++){
        auto group=*iter;
        for(int i=0;i<10;i++){
            map<int,vector<DFA*>> divide_group;
            for(auto dfa:group->group){
                partition(table,dfa,divide_group,char(i));
            }
            if(divide_group.size()>1){
                on_partition=true;
                group_list.erase(iter);
                add_group_list(group_list,divide_group);
                return;
            }
        }
    }
}
void DFABuilder::partition_on_char(vector<vector<int>>& jump_table){
    for(auto iter=group_list.begin();iter!=group_list.end();iter++){
        auto group=*iter;
        for(int i=0;i<127;i++){
            map<int,vector<DFA*>> divide_group;
            for(auto dfa:group->group){
                partition(table,dfa,divide_group,char(i));
            }
            if(divide_group.size()>1){
                on_partition=true;
                group_list.erase(iter);
                add_group_list(group_list,divide_group);
                return;
            }
        }
    }
}
void DFABuilder::partition_on_existence(vector<vector<int>>& jump_table){

}
void DFABuilder::add_group_list(vector<DFAGroup*>& group_list,map<int,vector<DFA*>>& divide_group){
    for(auto& item:divide_group){
        auto dfaGroup=new DFAGroup(trans_table);
        dfaGroup->group=item.second;
        group_list.push_back(dfaGroup);
    }
}
void DFABuilder::partition(vector<vector<int>>& jump_table,DFA* dfa,map<int,vector<DFA*>>& divide_group,char ch){
    int dest=jump_table[dfa->id_num][ch];
    auto dest_group=dfa_in_group(dest);
    if(!dest_group){
        if(divide_group.find(-1)==divide_group.end()){
            divide_group[-1]={};
        }
        divide_group[-1].push_back(dfa);
    }else{
        if(divide_group.find(dest_group->group_num)==divide_group.end()){
            divide_group[dest_group->group_num]={};
        }
        divide_group[dest_group->group_num].push_back(dfa);
    }
}
DFAGroup* DFABuilder::dfa_in_group(int status_num){
    for(auto group:group_list){
        for(auto dfa:group->group){
            if(dfa->id_num==status_num){
                return group;
            }
        }
    }
    return NULL;
}
void DFABuilder::create_mindfa_table(vector<vector<int>>& jump_table){
    for(auto dfa:st){
        int from_dfa=dfa->id_num;
        for(int i=0;i<127;i++){
            auto to_dfa=jump_table[from_dfa][char(i)];
            if(to_dfa!=-1){
                auto from_group=dfa_in_group(from_dfa);
                auto to_group=dfa_in_group(to_dfa);
                trans_table[from_group->group_num][char(i)]=to_group->group_num;
            }
        }
        if(dfa->accepted){
            auto from_group=dfa_in_group(from_dfa);
            trans_table[from_group->group_num][127]=1;
        }
    }
    jump_table=trans_table;
}
void DFABuilder::print_group(DFAGroup* group){
    for(auto dfa:group->group){
        log("   dfa sets: "+to_string(dfa->id_num));
    }
}
void DFABuilder::log_group(vector<DFAGroup*>& group_list){
    for(auto group:group_list){
        for(auto dfa:group->group){
            log("group_num: "+to_string(group->group_num));
            print_group(group);
        }
    }
}