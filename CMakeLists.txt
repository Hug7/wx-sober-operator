cmake_minimum_required(VERSION 3.24)
project(SOBER_OPERATOR CXX C)
export(PACKAGE SOBER_OPERATOR)

set(CMAKE_CXX_STANDARD 11)

#add_executable(sober_operator main.cpp common/assign/ModifiedJonkerVolgenant.cpp common/assign/ModifiedJonkerVolgenant.h)
#add_executable(SOBER_OPERATOR main.cpp common/assign/test/test_modified_jonker_volgenant.cpp)
add_executable(SOBER_OPERATOR main.cpp)

