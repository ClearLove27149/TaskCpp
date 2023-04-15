//
// Created by 26685 on 2023/4/15.
//
#include "../taskcpp/when_any.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <numeric>

void PrintThread() {
    cout << std::this_thread::get_id() << endl;
    std:this_thread::sleep_for(std::chrono::milliseconds(1));
}
void TestWhenAny() {
    vector<Task<int()>> v = {
            Task<int()>([]{PrintThread(); std::this_thread::sleep_for(std::chrono::seconds(1)); return 1;}),
            Task<int()>([]{PrintThread(); return 2;}),
            Task<int()>([]{PrintThread(); return 3;}),
            Task<int()>([]{PrintThread(); return 4;})
    };
    cout << "When Any" << endl;
    WhenAny(v).Then([] (std::pair<int,int> result) {
        cout << "index " << result.first << " result " << result.second << endl;
        return result.second;
    }).Then([] (int result) {
        cout << "Any result " << result << endl;
    }).Get();
}
int main() {
    TestWhenAny();
    return 0;
}