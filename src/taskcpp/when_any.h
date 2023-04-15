//
// Created by 26685 on 2023/4/15.
//

#ifndef TASKCPP_WHEN_ANY_H
#define TASKCPP_WHEN_ANY_H
#include "task.h"

#include <vector>
using namespace std;
using namespace taskcpp;

namespace detail {
    template<typename R>
    struct RangeTrait {
        typedef R type;
    };
    template<typename R>
    struct RangeTrait<std::shared_future<R>> {
        typedef R type;
    };
    template<typename Range>
    vector<std::shared_future<typename Range::value_type::return_type>> TransForm(Range& range) {
        typedef typename Range::value_type::return_type ReturnType;
        vector<std::shared_future<ReturnType>> fv;
        for (auto task : range) {
            fv.emplace_back(task.Run());
        }
        return fv;
    }
    template<typename Range>
    std::pair<int, typename RangeTrait<typename Range::value_type>::type> GetAnyResultPair(Range& fv) {
        size_t size = fv.size();
        for(;;) {
            for (size_t i = 0; i < size; ++i) {
                if(fv[i].wait_for(std::chrono::milliseconds(1)) == std::future_status::ready) {
                    return std::make_pair(i, fv[i].get());
                }
            }
        }
    }

}

template<typename Range>
auto WhenAny(Range& range) {
    typedef typename Range::value_type::return_type ReturnType;
    auto task = [&range] {
        using namespace detail;
        auto res = TransForm(range);
        return GetAnyResultPair(res);

    };
    return Task<std::pair<int, ReturnType>()>(task);
}


#endif //TASKCPP_WHEN_ANY_H
