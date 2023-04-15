// Created by 26685 on 2023/4/15.
//
#include "../taskcpp/parallel_for_each.h"
#include <chrono>
#include<iostream>

void check_prime(int x, std::vector<int>& res) {
    for (int i=2;i<x;i++) {
        if (x%i==0) {
            return;
        }
    }
    res.push_back(x);
}
bool check_prime_bool(int x) {
    for (int i=2;i<x;i++) {
        if (x%i==0) {
            return false;
        }
    }
    return true;
}
void get_all_prime(std::future<std::vector<int>>& each_future, std::vector<int>& res) {
    res.insert(res.end(), each_future.get().begin(), each_future.get().end());
}
void TestForeach() {
    std::vector<int> nums, res;
    for (int i=0;i<10000;i++) {
        nums.push_back(i+1);
    }
    auto start = std::chrono::high_resolution_clock::now();
    std::for_each(nums.begin(), nums.end(), [&] (int x) {
        check_prime(x, res);
    });
    std::cout << "res size : " << res.size() <<std::endl;
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double,std::milli> tm = end - start;
    std::cout << "for_each cost time : " << tm.count() << std::endl;
    //auto check = std::bind(check_prime,std::placeholders::_1, res);
    res.erase(res.begin(), res.end());
    start = std::chrono::high_resolution_clock::now();
    auto check = [&](int x) {
        check_prime(x, res);
    };
    // vector not thread security
    ParallelForeach(nums.begin(), nums.end(), check);
    std::cout << "res size : " << res.size() <<std::endl; // wrong
    end = std::chrono::high_resolution_clock::now();
    tm = end - start;
    std::cout << "ParallelForeach cost time : " << tm.count() << std::endl;
    //
    res.erase(res.begin(), res.end());
    start = std::chrono::high_resolution_clock::now();
    auto get_all = [&](std::shared_future<std::vector<int>>& each_future) {
        res.insert(res.end(), each_future.get().begin(), each_future.get().end());
    };

    ParallelForeachThread(nums.begin(), nums.end(), check_prime_bool, get_all);
    std::cout << res.size() << std::endl;
    end = std::chrono::high_resolution_clock::now();
    tm = end - start;
    std::cout << "ParallelForeachThread cost time : " << tm.count() << std::endl;
}
int main() {
    TestForeach();
    return 0;
}