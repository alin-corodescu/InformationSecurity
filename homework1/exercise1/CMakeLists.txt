cmake_minimum_required(VERSION 3.7)
project(exercise1)

set(CMAKE_CXX_STANDARD 11)

include_directories(/usr/local/ssl/include)


link_directories(/usr/local/ssl/lib/)
link_libraries(crypto dl)

set(SOURCE_FILES main.cpp)
add_executable(exercise1 ${SOURCE_FILES})
target_link_libraries(exercise1 ${SSL_LIBS})