cmake_minimum_required(VERSION 3.21)
project(exercise1)
find_package(Boost REQUIRED)
add_executable(my_app sync.cpp)
add_executable(my_app2 async.cpp)
target_include_directories(my_app PRIVATE ${Boost_INCLUDE_DIRS})
target_include_directories(my_app2 PRIVATE ${Boost_INCLUDE_DIRS})

