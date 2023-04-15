//
// Created by 26685 on 2023/4/15.
//

#ifndef TASKCPP_PARALLEL_REDUCE_H
#define TASKCPP_PARALLEL_REDUCE_H
#include <iostream>
#include <thread>
#include <vector>
#include <future>
#include <functional>

template<typename Range, typename ReduceFunc>
typename Range::value_type ParallelReduce(Range& range, typename Range::value_type& init, ReduceFunc reduceFunc) {
    return ParallelReduce<Range, ReduceFunc>(range, init, reduceFunc, reduceFunc);
}

template<typename Range, typename RangeFunc, typename ReduceFunc>
typename Range::value_type ParallelReduce(Range& range, typename Range::value_type& init, RangeFunc& rangeFunc, ReduceFunc& reduceFunc) {
    auto partNum = std::thread::hardware_concurrency();
    auto begin = std::begin(range);
    auto end = std::end(range);
    auto blockSize = std::distance(begin, end) / partNum;
    typename Range::iterator last = begin;
    if (blockSize > 0) {
        std::advance(last, (partNum-1)*blockSize);
    } else {
        last = end;
        blockSize = 1;
    }
    typedef typename Range::value_type ValueType;
    std::vector<std::future<ValueType>> futures;
    for (; begin != last; std::advance(begin, blockSize)) {
        futures.emplace_back(std::async([begin, &init, blockSize, &rangeFunc] {
            return rangeFunc(begin, begin+blockSize, init);
        }));
    }
    // last
    futures.emplace_back(std::async([&begin, &end, &init, &rangeFunc] {
        return rangeFunc(begin, end, init);
    }));

    std::vector<ValueType> results;
    std::for_each(futures.begin(), futures.end(), [&results] (std::future<ValueType>& each_future) {
        results.emplace_back(each_future.get());
    });
    return reduceFunc(results.begin(), results.end(), init);
}

template<typename Range, typename RangeFunc, typename ReduceFunc>
Range ParallelReduce(Range& range, Range& init, RangeFunc rangeFunc, ReduceFunc reduceFunc) {
    auto partNum = std::thread::hardware_concurrency();
    auto begin = std::begin(range);
    auto end = std::end(range);
    auto blockSize = std::distance(begin, end) / partNum;
    typename Range::iterator last = begin;
    if (blockSize > 0) {
        std::advance(last, (partNum - 1) * blockSize);
    } else {
        last = end;
        blockSize = 1;
    }
    typedef Range ValueType;
    std::vector<std::shared_future<ValueType>> futures;
    for (; begin != last; std::advance(begin, blockSize)) {
        futures.emplace_back(std::async([begin, &init, blockSize, &rangeFunc] {
            return rangeFunc(begin, begin + blockSize, init);
        }));
    }
    // last
    futures.emplace_back(std::async([&begin, &end, &init, &rangeFunc] {
        return rangeFunc(begin, end, init);
    }));

    std::vector<ValueType> results;
    std::for_each(futures.begin(), futures.end(), [&results](std::shared_future<ValueType> &each_future) {
        results.emplace_back(each_future.get());
    });
    return reduceFunc(results.begin(), results.end(), init);
}
#endif //TASKCPP_PARALLEL_REDUCE_H
