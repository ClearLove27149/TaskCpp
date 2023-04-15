//
// Created by 26685 on 2023/4/15.
//

#ifndef TASKCPP_PARALLEL_FOR_EACH_H
#define TASKCPP_PARALLEL_FOR_EACH_H

#include <thread>
#include <functional>
#include <vector>
#include <future>
#include <iostream>

template<typename Iterator, typename Function>
void ParallelForeach(Iterator& begin, Iterator& end, Function& func) {
    auto partNum = std::thread::hardware_concurrency();
    std::cout << "partNum :" << partNum << std::endl;
    auto blockSize = std::distance(begin, end) / partNum;
    Iterator last = begin;
    if (blockSize > 0) {
        std::advance(last, (partNum - 1)*blockSize);
    } else {
        last = end;
        blockSize = 1;
    }
    std::vector<std::future<void>> futures;
    // [0, n-1] block
    for (; begin != last; std::advance(begin, blockSize)) {
        futures.emplace_back(std::async([begin, blockSize, &func] {
            std::for_each(begin, begin + blockSize, func);
        }));
    }
    // last block
    futures.emplace_back(std::async([&begin, &end, &func] {
        std::for_each(begin, end, func);
    }));

    std::for_each(futures.begin(), futures.end(), [] (std::future<void>& each_future) {
        each_future.get();
    });
}
template<typename Iterator, typename Function>
void ParallelForeach(Iterator begin, Iterator end, Function& func) {
    auto begin_tmp = begin;
    auto partNum = std::thread::hardware_concurrency();
    std::cout << "partNum :" << partNum << std::endl;
    auto blockSize = std::distance(begin, end) / partNum;
    Iterator last = begin;
    if (blockSize > 0) {
        std::advance(last, (partNum-1)*blockSize);
    } else {
        last = end;
        blockSize = 1;
    }
    std::vector<std::future<void>> futures;
    // [0, n-1] block
    for (; begin != last; std::advance(begin, blockSize)) {
        //std::cout << "begin: "<<std::distance(begin_tmp, begin) << ", " <<std::distance(begin_tmp, begin+blockSize)<<std::endl;
        futures.emplace_back(std::async([begin, blockSize, func] {
            std::for_each(begin, begin + blockSize, func);
        }));
    }
    // last block
    //std::cout << "begin: "<<std::distance(begin_tmp, begin) << ", " <<std::distance(begin_tmp, end)<<std::endl;
    futures.emplace_back(std::async([begin, end, func] {
        std::for_each(begin, end, func);
    }));

    std::for_each(futures.begin(), futures.end(), [] (std::future<void>& each_future) {
        each_future.get();
    });
}
// example
// map = check
// reduce = [](res) {res.push)
template<typename Iterator, typename Map, typename Reduce>
void ParallelForeachThread(Iterator begin, Iterator end, Map& map, Reduce& reduce) {
    auto begin_tmp = begin;
    auto partNum = std::thread::hardware_concurrency();
    std::cout << "partNum :" << partNum << std::endl;
    auto blockSize = std::distance(begin, end) / partNum;
    Iterator last = begin;
    if (blockSize > 0) {
        std::advance(last, (partNum-1)*blockSize);
    } else {
        last = end;
        blockSize = 1;
    }
    // get reduce args type

    std::vector<std::shared_future<std::vector<int>>> futures;
    // [0, n-1] block
    for (; begin != last; std::advance(begin, blockSize)) {
        //std::cout << "begin: "<<std::distance(begin_tmp, begin) << ", " <<std::distance(begin_tmp, begin+blockSize)<<std::endl;
        futures.emplace_back(std::async([begin, blockSize, map] {
            std::vector<int> tmp;
            std::for_each(begin, begin + blockSize, [&](int x) {
                if (map(x)) {
                    tmp.push_back(x);
                }
            });
            return tmp;
        }));
    }
    // last block
    //std::cout << "begin: "<<std::distance(begin_tmp, begin) << ", " <<std::distance(begin_tmp, end)<<std::endl;
    futures.emplace_back(std::async([begin, end, map] {
        std::vector<int> last_tmp;
        std::for_each(begin, end, [&](int x) {
            if (map(x)) {
                last_tmp.push_back(x);
            }
        });
        return last_tmp;
    }));
    std::cout << "ok" << std::endl;
//    std::for_each(futures.begin(), futures.end(), [](std::future<std::vector<int>>& vec) {
//        std::cout << vec.get().size() << std::endl;
//    });
    std::for_each(futures.begin(), futures.end(), reduce);
}
#endif //TASKCPP_PARALLEL_FOR_EACH_H
