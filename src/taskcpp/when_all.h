//
// Created by 26685 on 2023/4/15.
//

#ifndef TASKCPP_WHEN_ALL_H
#define TASKCPP_WHEN_ALL_H

#include "task.h"
#include "task_group.h"

#include <vector>

using namespace std;
using namespace taskcpp;

template<typename Range>
auto WhenAll(Range& range) {
    typedef typename Range::value_type::return_type ReturnType;
    auto task = [&range] {
        vector<std::shared_future<ReturnType>> fv;
        for (auto task : range) {
            fv.emplace_back(task.Run());
        }

        vector<ReturnType> v;
        for (auto item : fv) {
            v.emplace_back(item.get());
        }
        return v;
    };
    return Task<vector<ReturnType>()>(task);
}

#endif //TASKCPP_WHEN_ALL_H
