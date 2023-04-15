//
// Created by 26685 on 2023/4/15.
//
#include "../taskcpp/parallel_invoke.h"

void TestInvoke() {
    auto f1 = [] {std::cout<< 1 << std::endl;};
    std::function<void()>  f2 = [] {std::cout <<"func" << std::endl;};
    ParallelInvoke(f1, f2, []{std::cout << 2 << std::endl;});
}
int main() {
    TestInvoke();
    return 0;
}