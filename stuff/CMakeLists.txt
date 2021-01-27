cmake_minimum_required(VERSION 3.10)
project(stuff)

set(CMAKE_CXX_STANDARD 17)

add_executable(stuff main.cpp tnf_tree.cpp uniq.cpp tnf_tree.h var_visit.cpp)

#set(SOURCE
#    ${SOURCE}
#    ${CMAKE_CURRENT_SOURCE_DIR}/uniq.cpp
#    ${CMAKE_CURRENT_SOURCE_DIR}/tnf_tree.cpp
#)