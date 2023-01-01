#include<iostream>
#include "nfa.h"
#include<string>
using namespace std;

int NFA::counter=0;


NFA::NFA(){
    id_num=counter++;
}

void NFA::display(){
    if(this->link1){
        log("from: "+to_string(this->id_num));
        log("edge: "+showEdgeInfo(this->type,this->edge));
        log("to: "+to_string(this->link1->id_num));
    }
    if(this->link2){
        log("from: "+to_string(this->id_num));
        log("edge: "+showEdgeInfo(this->type,this->edge));
        log("to: "+to_string(this->link2->id_num));
    }
    if(this->link1){
        this->link1->display();
    }
    if(this->link2){
        this->link2->display();
    }
}

string NFA::showEdgeInfo(Category cat,char edge){
    string ret;
    switch(cat){
        case EMPTY:
            ret="epsilon";
            break;
        case CHAR:
            ret="char "+edge;
            break;
        case SETS:
            ret="sets";
            break;
        case ANYCHAR:
            ret="any";
            break;
    }
    return ret;
}

NFABuilder::NFABuilder(string pat){
    lex=Lexer(pat);
    nfa_pair=make_pair(new NFA(),new NFA());
}

NFA* NFABuilder::build(){
    lex.advance();
    group(nfa_pair);
    return nfa_pair.first;
}

void NFABuilder::term(pair<NFA*,NFA*>& npair){
    if(lex.match(Token::L)){
        nfa_sigchar(npair);
    }else if(lex.match(Token::ANY)){
        nfa_any_char(npair);
    }else if(lex.match(Token::CCL_START)){
        nfa_char_set(npair);
    }

}
bool NFABuilder::nfa_sigchar(pair<NFA*,NFA*>& npair){
    if(!lex.match(Token::L)){
        return false;
    }
    npair.first->link1=npair.second;
    npair.first->edge=lex.letter;
    npair.first->type=CHAR;
    lex.advance();
    return true;
}
bool NFABuilder::nfa_any_char(pair<NFA*,NFA*>& npair){
    if(!lex.match(Token::ANY)){
        return false;
    }
    npair.first->link1=npair.second;
    npair.first->type=ANYCHAR;
    lex.advance();
    return true;
}
bool NFABuilder::nfa_char_set(pair<NFA*,NFA*>& npair){
    if(!lex.match(Token::CCL_START)){
        return false;
    }
    bool inverse=false;
    lex.advance();
    if(lex.match(Token::AT_BOL)){
        inverse=true;
        lex.advance();
    }

    npair.first=new NFA();
    npair.second=new NFA();
    npair.first->link1=npair.second;
    npair.first->type=SETS;
    dash(npair.first->st);
    if(inverse){
        set<char> st;
        auto& nfast=npair.first->st;
        for(int i=0;i<127;i++){
            if(nfast.find(char(i))==nfast.end()){
                st.insert(char(i));
            }
        }
        nfast=st;
    }
    lex.advance();
    return true;
}
void NFABuilder::dash(set<char>& st){
    char letter;
    while(!lex.match(Token::CCL_END)){
        if(!lex.match(Token::DASH)){
            letter=lex.letter;
            st.insert(lex.letter);
        }else{
            lex.advance();
            for(char c=letter;c<=lex.letter;c++){
                st.insert(c);
            }
        }
        lex.advance();
    }
}
bool NFABuilder::factor_conn(pair<NFA*,NFA*>& npair){
    if(isconn(lex.cur_token)){
        factor(npair);
    }
    while(isconn(lex.cur_token)){
        pair<NFA*,NFA*> np=pair<NFA*,NFA*>{new NFA(),new NFA()};
        factor(np);
        npair.second->link1=np.first;
        npair.second=np.second;
    }
    return true;
}
bool NFABuilder::isconn(Token tk){
    auto connset=set<Token>();
    connset={
        Token::OPEN_PAREN,
        Token::CLOSE_PAREN,
        Token::AT_EOL,
        Token::EOS,
        Token::CLOSURE,
        Token::PLUS_CLOSE,
        Token::CCL_END,
        Token::AT_BOL,
        Token::OR
    };
    if(connset.find(tk)==connset.end()){
        return true;
    }else{
        return false;
    }
}
bool NFABuilder::factor(pair<NFA*,NFA*>& npair){
    term(npair);
    if(lex.match(Token::CLOSURE)){
        closure(npair);
    }else if(lex.match(Token::PLUS_CLOSE)){
        plus_closure(npair);
    }else if(lex.match(Token::OPTIONAL)){
        option_closure(npair);
    }
    return true;
}
bool NFABuilder::closure(pair<NFA*,NFA*>& npair){
    if(!lex.match(Token::CLOSURE)){
        return false;
    }
    auto start=new NFA();
    auto end=new NFA();
    start->link1=npair.first;
    start->link2=end;
    npair.second->link1=npair.first;
    npair.second->link2=end;
    npair.first=start;
    npair.second=end;
    lex.advance();
    return true;
}
bool NFABuilder::plus_closure(pair<NFA*,NFA*>& npair){
    if(!lex.match(Token::PLUS_CLOSE)){
        return false;
    }
    auto start=new NFA();
    auto end=new NFA();
    start->link1=npair.first;
    npair.second->link1=npair.first;
    npair.second->link2=end;
    npair.first=start;
    npair.second=end;
    lex.advance();
    return true;
}
bool NFABuilder::option_closure(pair<NFA*,NFA*>& npair){
    if(!lex.match(Token::OPTIONAL)){
        return false;
    }
    auto start=new NFA();
    auto end=new NFA();
    start->link1=npair.first;
    start->link2=end;
    npair.second->link1=end;
    npair.first=start;
    npair.second=end;
    lex.advance();
    return true;
}
bool NFABuilder::expr(pair<NFA*,NFA*>& npair){
    factor_conn(npair);
    pair<NFA*,NFA*> newpair=pair<NFA*,NFA*>(new NFA(),new NFA());
    while(lex.match(Token::OR)){
        lex.advance();
        factor_conn(newpair);
        auto start=new NFA();
        start->link1=newpair.first;
        start->link2=npair.first;
        npair.first=start;
        auto end=new NFA();
        newpair.second->link1=end;
        npair.second->link2=end;
        npair.second=end;
    }
    return true;
}
bool NFABuilder::group(pair<NFA*,NFA*>& npair){
    if(lex.match(Token::OPEN_PAREN)){
        lex.advance();
        expr(npair);
        if(lex.match(Token::CLOSE_PAREN)){
            lex.advance();
        }
    }else if(lex.match(Token::EOS)){
        return false;
    }else{
        expr(npair);
    }
    while(true){
        pair<NFA*,NFA*> newpair=pair<NFA*,NFA*>(new NFA(),new NFA());
        if(lex.match(Token::OPEN_PAREN)){
            lex.advance();
            expr(newpair);
            npair.second->link1=newpair.first;
            npair.second=newpair.second;
            if(lex.match(Token::CLOSE_PAREN)){
                lex.advance();
            }
        }else if(lex.match(Token::EOS)){
            return false;
        }else{
            expr(newpair);
            npair.second->link1=newpair.first;
            npair.second=newpair.second;
        }
    }
}

void move(set<NFA*>& st,char ch){
    auto out_set=set<NFA*>();
    for(auto it:st){
        if(it->type==CHAR&&it->edge==ch||it->type==ANYCHAR||it->type==SETS&&it->st.find(ch)!=it->st.end()){
            out_set.insert(it->link1);
        }
    }
    st=out_set;
}