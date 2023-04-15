//
// Created by 26685 on 2023/4/15.
//
#include "../taskcpp/task.h"
#include <iostream>
using namespace taskcpp;
using namespace std;

void task_then() {
    Task<int()> t([] {return 32;} );
    auto r1 = t.Then([](int result) {
        cout<<result<<endl; return result+3;
    }).Then([](int result) {
        cout << result << endl;
        return result + 3;
    }).Get();
    cout<<r1<<endl;
    //
    Task<int(int)> t2([](int i) {return i;} );
    auto r2 = t2.Then([](int result) {
        cout<<result<<endl; return result+3;
    }).Then([](int result) {
        cout << result << endl;
        return result + 3;
    }).Get(1);
    cout<<r2<<endl;
}
int main() {
    task_then();
    return 0;
}