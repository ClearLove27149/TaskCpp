cmake_minimum_required(VERSION 3.24)
project(TaskCpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(TaskCpp main.cpp)

add_executable(test_group src/test/test_taskgroup.cpp src/taskcpp/task_group.h src/taskcpp/task.h)
add_executable(test_task src/test/test_task.cpp src/taskcpp/task.h)
add_executable(test_when_all src/test/test_when_all.cpp src/taskcpp/when_all.h src/taskcpp/task.h)
add_executable(test_when_any src/test/test_when_any.cpp src/taskcpp/when_any.h src/taskcpp/task.h)
add_executable(test_for_each src/test/test_parallel_for_each.cpp src/taskcpp/parallel_for_each.h)
add_executable(test_for_invoke src/test/test_parallel_invoke.cpp src/taskcpp/parallel_invoke.h)
add_executable(test_reduce src/test/test_parallel_reduce.cpp src/taskcpp/parallel_reduce.h)

add_executable(test src/taskcpp2/test.cpp src/taskcpp2/TaskGroup.hpp src/taskcpp2/Task.hpp)
