#include"regex.h"

int main(){
    clock_t start,end;
    start=clock();
    vector<Sample> samples={
        {"T6","([A-Z]+[0-9]+)",true},
        {"THISISREGEXTEST","([A-Z]+[0-9]+)",false},
        {"234234abcdefg[*+","([A-Z]+[0-9]*abcdefg)(\\[\\*\\+)",false},
        {"AS342abcdefg234aaaaabccccczczxczcasdzxc","([A-Z]+[0-9]*abcdefg)([0-9]*)(\\*?|a+)(zx|bc*)([a-z]+|[0-9]*)(asd|fgh)(zxc)",true},
        {"abc","[^0-9]*",true},
        {"abbbbb","[^c]+", true},
        {"fee","fee|fie", true},
    };
    for(auto it:samples){//NFA测试
        Regex reg(it.pattern);
        if(reg.match(it.input_str)==it.result){
            log("RIGHT");
        }else{
            log("WRONG");
        }
    }
    for(auto it:samples){//DFA测试
        Regex reg(it.pattern,2);
        if(reg.match(it.input_str)==it.result){
            log("RIGHT");
        }else{
            log("WRONG");
        }
    }
    for(auto it:samples){//DFA minimize测试
        Regex reg(it.pattern,2,true);
        if(reg.match(it.input_str)==it.result){
            log("RIGHT");
        }else{
            log("WRONG");
        }
    }
    end=clock();
    cout<<(double)(end-start)/CLOCKS_PER_SEC<<endl;
    return 0;
}