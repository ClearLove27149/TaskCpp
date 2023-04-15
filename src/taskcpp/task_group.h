#pragma once
#ifndef TASKCPP_TASK_GROUP_H
#define TASKCPP_TASK_GROUP_H

#include "task.h"
#include <vector>
#include <map>
#include <string>
#include <any>
#include <variant>
#include <iostream>

namespace taskcpp {

    class TaskGroup {
        typedef std::variant<int, std::string,double,short,unsigned int> RetVariant;
    public:
        TaskGroup() { }
        ~TaskGroup() { }

        template<typename R, typename = typename std::enable_if<!std::is_same<R, void>::value>::type>
        void Run(Task<R()>&& task) {
            m_group.emplace(R(), task.Run());
        }
        template<typename R, typename = typename std::enable_if<!std::is_same<R, void>::value>::type>
        void Run(Task<R()>& task) {
            m_group.emplace(R(), task.Run());
        }

        void Run(Task<void()>& task) {
            m_voidGroup.push_back(task.Run());
        }
        void Run(Task<void()>&& task) {
            m_voidGroup.push_back(task.Run());
        }

        template<typename F>
        void Run(F&& f) {
            Run(Task<typename std::result_of<F()>::type()>(std::forward<F>(f)));
        }

        template<typename F, typename... Funs>
        void Run(F&& first, Funs&&... rest) {
            Run(std::forward<F>(first));
            Run(std::forward<Funs>(rest)...);
        }
        /**
         *
         */

        void Wait() {
            for (auto it = m_group.begin();it!=m_group.end();++it) {
                auto vrt = it->first;
                if (std::get_if<int>(&vrt)) {
                    FutureGet<int>(it->second);
                } else if (std::get_if<std::string>(&vrt)) {
                    FutureGet<std::string>(it->second);
                } else if (std::get_if<double>(&vrt)) {
                    FutureGet<double>(it->second);
                } else if (std::get_if<short>(&vrt)) {
                    FutureGet<short>(it->second);
                } else {
                    FutureGet<unsigned int>(it->second);
                }
//                std::visit([&] (auto) {
//
//                    FutureGet<std::string>(it->second);
//                }, vrt);
                //std::get_if<int>(vrt);
//                std::visit(
//                        [&](int a) {FutureGet<int>(it->second);},
//                        [&](std::string a) {FutureGet<std::string>(it->second);},
//                        [&](double a) {FutureGet<double>(it->second);},
//                        [&](short a) {FutureGet<short>(it->second);},
//                        [&](unsigned int a) {FutureGet<unsigned int>(it->second);},
//                        vrt);


            }
            for (auto it = m_voidGroup.begin(); it!=m_voidGroup.end();++it) {
                it->get();
            }
        }

    private:
        template<class T>
        void FutureGet(std::any& f) {
            std::any_cast<std::shared_future<T>>(f).get();
        }
        std::vector<std::shared_future<void>> m_voidGroup;
        std::multimap<RetVariant, std::any> m_group;
    };
}
#endif //TASKCPP_TASK_GROUP_H
