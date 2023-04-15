//
// Created by 26685 on 2023/4/15.
//

#ifndef TASKCPP_PARALLEL_INVOKE_H
#define TASKCPP_PARALLEL_INVOKE_H

#include "../taskcpp/task_group.h"

using namespace taskcpp;
template<typename... Funcs>
void ParallelInvoke(Funcs&&... funcs) {
    TaskGroup taskGroup;
    taskGroup.Run(std::move(funcs)...);
    taskGroup.Wait();
}
#endif //TASKCPP_PARALLEL_INVOKE_H
