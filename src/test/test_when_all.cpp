//
// Created by 26685 on 2023/4/15.
//
#include "../taskcpp/when_all.h"

#include <thread>
#include <chrono>
#include <numeric>

void PrintThread() {
    cout << std::this_thread::get_id() << endl;
    std:this_thread::sleep_for(std::chrono::milliseconds(1));
}
void TestWhenAll() {
    vector<Task<int()>> v = {
            Task<int()>([]{PrintThread(); std::this_thread::sleep_for(std::chrono::seconds(1)); return 1;}),
            Task<int()>([]{PrintThread(); return 2;}),
            Task<int()>([]{PrintThread(); return 3;}),
            Task<int()>([]{PrintThread(); return 4;})
    };
    cout << "When All" << endl;
    WhenAll(v).Then([] (vector<int> result) {
        cout << "The sum is " << std::accumulate(begin(result), end(result), 0)<<endl;
    }).Wait();
}
int main() {
    TestWhenAll();
    return 0;
}