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

class DFAGroup{
    public:
        static int counter;
        int group_num;
        vector<DFA*> group;
        DFAGroup(vector<vector<int>>& trans_table);//trans_table的容量不能小于DFAGroup结点个数
};

class DFABuilder{
    public:
        DFABuilder(string pattern="",bool minimize=false);
        DFA* nfa_to_dfa(set<NFA*>& st);
        DFA* repeated_dfa(set<NFA*>& st);//获取已存在的dfa
        vector<DFA*> st;
        vector<vector<int>> table;
        vector<vector<int>> trans_table;
        vector<DFAGroup*> group_list;
        bool on_partition;
        string pattern;
        bool minimize=false;
        void gen_dfa_table(NFA* start);
        bool repeated(set<NFA*>& closure_st);
        bool dfa_match(string input_str);
        void build();
        void minimize_dfa(vector<vector<int>>& jump_table);
        void partition_accepted();
        void partition_on_num(vector<vector<int>>& jump_table);
        void add_group_list(vector<DFAGroup*>& group_list,map<int,vector<DFA*>>& divide_group);
        void partition_on_char(vector<vector<int>>& jump_table);
        void partition_on_existence(vector<vector<int>>& jump_table);//只对nfa中可接受的字符进行遍历，降低时间复杂度
        void partition(vector<vector<int>>& jump_table,DFA* dfa,map<int,vector<DFA*>>& divide_group,char ch);
        DFAGroup* dfa_in_group(int status_num);
        void create_mindfa_table(vector<vector<int>>& jump_table);
        void print_group(DFAGroup* group);
        void log_group(vector<DFAGroup*>& group_list);
};
void closure_set(set<NFA*>& st);