#include"global.h"
#include<set>
#include"nfa.h"
#include<vector>
#include<stack>
#include<map>
using namespace std;
class DFA{
    public:
        set<NFA*> nfast;
        bool accepted=false;
        int id_num;
        static int counter;
};

class DFABuilder{
    public:
        DFABuilder(string pattern="");
        DFA* nfa_to_dfa(set<NFA*>& st);
        DFA* repeated_dfa(set<NFA*>& st);//获取已存在的dfa
        vector<DFA*> st;
        vector<vector<int>> table;
        string pattern;
        bool minimize=false;
        void gen_dfa_table(NFA* start);
        bool repeated(set<NFA*>& closure_st);
        bool dfa_match(string input_str);
        void build();
};
void closure_set(set<NFA*>& st);