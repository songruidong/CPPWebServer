cmake_minimum_required(VERSION 3.10)
project(my_project)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CXXFLAGS "-g")
if(DEBUG)
    set(CXXFLAGS "${CXXFLAGS} -O0")
else()
    set(CXXFLAGS "${CXXFLAGS} -O2")
endif()

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)
# 包含MySQL头文件目录
include_directories(/usr/include/mysql /usr/lib/mysql /usr/include/mysql/mysql)
include_directories(./timer/ ./http/ ./log/ ./CGImysql/ ./threadpool/ ./timer/)
add_executable(server main.cpp ./timer/lst_timer.cpp ./http/http_conn.cpp ./log/log.cpp ./CGImysql/sql_connection_pool.cpp  webserver.cpp config.cpp)
target_link_libraries(server  Threads::Threads   mysqlclient)



