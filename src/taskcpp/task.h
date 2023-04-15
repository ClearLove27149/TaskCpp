//
// Created by 26685 on 2023/4/8.
//
#pragma once
#ifndef TASKCPP_TASK_H
#define TASKCPP_TASK_H

#include <functional>
#include <future>

namespace taskcpp {
    template<typename T>
    class Task;

    template<typename R, typename... Args>
    class Task<R(Args...)> {
        std::function<R(Args...)> m_fn;

    public:
        typedef R return_type;

        // PPL
        template<typename F>
        //typename R, typename = typename std::enable_if<!std::is_same<R, void>::value>::type>
        //auto Then(F&& f) -> typename std::enable_if<!std::is_same<R, void>::value , typename std::result_of<F(R)>::type(Args...)> {
        auto Then(F&& f) {//-> Task<typename std::result_of<F(R)>::type(Args...)> {
            typedef typename std::result_of<F(R)>::type Return_type;
            auto func = std::move(m_fn);
            return Task<Return_type(Args...)>([func, &f] (Args&&... args) {
                std::future<R> lastf = std::async(func, std::forward<Args>(args)...);
                return std::async(f, lastf.get()).get();
            });
        }

        Task(std::function<R(Args...)>& f) : m_fn(f) { }
        Task(std::function<R(Args...)>&& f) : m_fn(std::move(f)) { }
//        Task(std::function<void()>& f) : m_fn(f) { }
//        Task(std::function<void()>&& f) : m_fn(std::move(f)) { }

        ~Task() { }
        void Wait() {
            std::async(m_fn).wait();
        }

        template<class... TArgs>
        R Get(TArgs&&... args) {
            return std::async(m_fn, std::forward<TArgs...>(args)...).get();
        }

        std::shared_future<R> Run() {
            return std::async(m_fn);
        }
    };
}
#endif //TASKCPP_TASK_H
