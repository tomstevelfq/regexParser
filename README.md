# Regex Parser

![](https://img.shields.io/badge/language-c%2B%2B-blue)
![](https://img.shields.io/badge/category-regex%20parser-green)
![](https://img.shields.io/badge/system-windows-yellow)

> This is a regex parser implementing by c++.

## how to start
```c++
#include "regex.h"
#include<iostream>
using namespace std;

int main(){
    string pattern = "abc|ade";
    string input_str = "ade";
    Regex reg(it.pattern);//NFA
    Regex reg1(it.pattern,2);//DFA
    Regex reg2(it.pattern,2,True);//DFA minimize
    cout<<reg.match(input_str)<<endl;
    cout<<reg1.match(input_str)<<endl;
    cout<<reg2.match(input_str)<<endl;
    return 0;
}
```
