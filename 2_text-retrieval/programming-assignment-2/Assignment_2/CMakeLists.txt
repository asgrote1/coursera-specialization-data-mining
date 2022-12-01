cmake_minimum_required(VERSION 3.2.0)

# MeTA only requires C++11, but on all of our supported compilers at least
# C++1y support exists, so we therefore set the standard to 14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(../meta ${CMAKE_CURRENT_BINARY_DIR}/meta EXCLUDE_FROM_ALL)

add_executable(competition competition.cpp)
target_link_libraries(competition meta-index
                                  meta-ranker
                                  meta-sequence-analyzers
                                  meta-parser-analyzers)

add_executable(new-judgements new-judgements.cpp)
target_link_libraries(new-judgements meta-index
                                     meta-ranker
                                     meta-sequence-analyzers
                                     meta-parser-analyzers)
