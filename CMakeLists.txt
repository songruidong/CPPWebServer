cmake_minimum_required(VERSION 3.10)
project(my_project)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 设置默认的构建类型为 Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# 自定义调试选项
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG")

# 查找线程库
find_package(Threads REQUIRED)

# 手动包含 MySQL 头文件目录
include_directories(/usr/include/mysql /usr/lib/mysql /usr/include/mysql/mysql)

# 添加可执行文件
add_executable(server
    main.cpp
    ./timer/lst_timer.cpp
    ./http/http_conn.cpp
    ./log/log.cpp
    ./CGImysql/sql_connection_pool.cpp
    webserver.cpp
    config.cpp
)

# 链接线程库和 MySQL 库
target_link_libraries(server
    Threads::Threads
    mysqlclient
)
