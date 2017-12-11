cmake_minimum_required(VERSION 3.7)
project(exercise1)

set(CMAKE_CXX_STANDARD 11)

include_directories(/usr/local/ssl/include)


link_directories(/usr/local/ssl/lib/)
link_libraries(crypto dl)

add_executable(exercise1 ex1.cpp)
target_link_libraries(exercise1 ${SSL_LIBS})

add_executable(main main.cpp)
target_link_libraries(main ${SSL_LIBS})

add_executable(exercise2 ex2.cpp)
target_link_libraries(exercise2 ${SSL_LIBS})

add_executable(testConf test.cpp)
target_link_libraries(testConf ${SSL_LIBS})

add_executable(A A.cpp ex3.h ex3.cpp)
target_link_libraries(A ${SSL_LIBS})

add_executable(B B.cpp ex3.h ex3.cpp)
target_link_libraries(B ${SSL_LIBS})

add_executable(KM KM.cpp ex3.h ex3.cpp)
target_link_libraries(KM ${SSL_LIBS})


