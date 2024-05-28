cmake_minimum_required(VERSION 3.24)
project(wx_sober_operator)

set(CMAKE_CXX_STANDARD 11)

add_executable(wx_sober_operator main.cpp common/ModifiedJonkerVolgenant.cpp common/ModifiedJonkerVolgenant.h)

