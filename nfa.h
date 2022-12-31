#ifndef NFAH
#define NFAH
#include<set>
#include<string>
#include"lexer.h"
#include"global.h"
using namespace std;
class NFA{
    public:
        Category type=EMPTY;
        set<char> st;
        char edge;
        NFA* link1=nullptr;
        NFA* link2=nullptr;
        static int counter;
        int id_num;
        NFA();
        void display();
        string showEdgeInfo(Category cat,char edge);
};

class NFABuilder{
    public:
        Lexer lex;
        pair<NFA*,NFA*> nfa_pair;
        NFABuilder(string pat="");
        NFA* build();
        void term(pair<NFA*,NFA*>& npair);
        bool nfa_sigchar(pair<NFA*,NFA*>& npair);
        bool nfa_any_char(pair<NFA*,NFA*>& npair);
        bool nfa_char_set(pair<NFA*,NFA*>& npair);
        bool factor_conn(pair<NFA*,NFA*>& npair);
        bool isconn(Token tk);
        bool factor(pair<NFA*,NFA*>& npair);
        void dash(set<char>& st);
        bool closure(pair<NFA*,NFA*>& pair);
        bool plus_closure(pair<NFA*,NFA*>& pair);
        bool option_closure(pair<NFA*,NFA*>& pair);
        bool expr(pair<NFA*,NFA*>& pair);
        bool group(pair<NFA*,NFA*>& pair);
};
#endif