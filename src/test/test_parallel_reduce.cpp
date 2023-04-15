//
// Created by 26685 on 2023/4/15.
//
#include "../taskcpp/parallel_reduce.h"
#include <numeric>
// 根据parallel_reduce重写求素数区间
// 本质上跟parallel_for_each版本差不多
bool check_prime_bool(int x) {
    for (int i = 2; i < x; i++) {
        if (x % i == 0) {
            return false;
        }
    }
    return true;
}

void TestPrimeReduce() {
    std::vector<int> nums, res;
    for (int i=0;i<10000;i++) {
        nums.push_back(i+1);
    }

    auto r = ParallelReduce(nums, res, [](const std::vector<int>::iterator& begin, const std::vector<int>::iterator& end, std::vector<int> tmp) {
        std::for_each(begin, end, [&tmp](int x)  {
            if (check_prime_bool(x)) {
                tmp.push_back(x);
            }
        });
        return tmp;
    },
    [](const std::vector<std::vector<int>>::iterator& begin, const std::vector<std::vector<int>>::iterator& end, std::vector<int> res1) {
        std::for_each(begin, end, [&res1](std::vector<int>& it) {
            res1.insert(res1.end(), it.begin(), it.end());
        });
        return res1;
    });
    std::cout << r.size() << std::endl;
}
void TestFindString() {
    std::vector<int> v;
    v.reserve(10000);
    for (int i=0;i<10000;i++) {
        v.push_back(i+1);
    }
    int i=0;
    auto r = ParallelReduce(v, i, [](const std::vector<int>::iterator& begin, const std::vector<int>::iterator& end, int val) {
        return std::accumulate(begin, end, val);
    });
    std::cout<<r<<std::endl;
}


int main() {
    TestFindString();
    TestPrimeReduce();
    return 0;
}