//
// Created by 26685 on 2023/4/9.
//
#include "TaskGroup.hpp"

using namespace Cosmos;
int main() {
    TaskGroup g;
    //Task<void()> ff([] { return 1;});
    std::function<int()> f = [] { return 1;};
    g.Run(f);
    g.Run(f, [] {std::cout <<"ok1"<<std::endl;},[]{std::cout<<"ok2"<<std::endl;});
    g.Wait();
}