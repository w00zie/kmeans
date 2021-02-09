project(tester)

cmake_minimum_required(VERSION 2.8.0)

set (CMAKE_CXX_FLAGS "-O3 -pthread")

add_executable(2d_500_full 500/full_iterations.cpp)
add_executable(2d_500_eps 500/with_epsilon.cpp)

add_executable(2d_1000_full 1000/full_iterations.cpp)
add_executable(2d_1000_eps 1000/with_epsilon.cpp)

add_executable(2d_2000_full 2000/full_iterations.cpp)
add_executable(2d_2000_eps 2000/with_epsilon.cpp)

add_executable(2d_5000_full 5000/full_iterations.cpp)
add_executable(2d_5000_eps 5000/with_epsilon.cpp)

add_executable(2d_10000_full 10000/full_iterations.cpp)
add_executable(2d_10000_eps 10000/with_epsilon.cpp)

add_executable(2d_50000_full 50000/full_iterations.cpp)
add_executable(2d_50000_eps 50000/with_epsilon.cpp)

add_executable(2d_100000_full 100000/full_iterations.cpp)
add_executable(2d_100000_eps 100000/with_epsilon.cpp)


enable_testing()

add_test(NAME 2d_500_full COMMAND 2d_500_full 20 12)
add_test(NAME 2d_500_eps COMMAND 2d_500_eps 20 12)

add_test(NAME 2d_1000_full COMMAND 2d_1000_full 20 12)
add_test(NAME 2d_1000_eps COMMAND 2d_1000_eps 20 12)

add_test(NAME 2d_2000_full COMMAND 2d_2000_full 20 12)
add_test(NAME 2d_2000_eps COMMAND 2d_2000_eps 20 12)

add_test(NAME 2d_5000_full COMMAND 2d_5000_full 20 12)
add_test(NAME 2d_5000_eps COMMAND 2d_5000_eps 20 12)

add_test(NAME 2d_10000_full COMMAND 2d_10000_full 20 12)
add_test(NAME 2d_10000_eps COMMAND 2d_10000_eps 20 12)

add_test(NAME 2d_50000_full COMMAND 2d_50000_full 20 12)
add_test(NAME 2d_50000_eps COMMAND 2d_50000_eps 20 12)

add_test(NAME 2d_100000_full COMMAND 2d_100000_full 20 12)
add_test(NAME 2d_100000_eps COMMAND 2d_100000_eps 20 12)


