# 指定 CMake 最低版本要求（3.2 5 以上对 C++20 支持更好）
cmake_minimum_required(VERSION 3.25)

# 设置 C++ 标准为 C++20，并要求必须支持
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)  # 可以禁用编译器扩展，保证可移植性，这里打开

# 项目名称、版本、描述和语言
project(pans
    VERSION 1.0.0
    DESCRIPTION "A modern C++20 project template"
    LANGUAGES CXX
)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers) 

# 导出 compile_commands.json（供 clangd、ccls 等工具使用）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 默认构建类型为 Debug（若未指定）
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

# 设置线程数为 CPU 核心数的 1/3 + 1
include(ProcessorCount)
ProcessorCount(CPU_COUNT)
math(EXPR THREAD_COUNT "1 + ${CPU_COUNT} / 2")
message(STATUS "CPU cores    : ${CPU_COUNT}")
message(STATUS "Thread count : ${THREAD_COUNT}  (1 + cores / 2)")


option(PANS_BUILD_EXAMPLES "Build examples" ON)
option(PANS_BUILD_TESTS "Build tests" ON)
option(PANS_BUILD_TOOLS "Build tools" ON)


## 添加自定义 CMake 模块路径
include(cmake/pansOptions.cmake)



add_subdirectory(pans)
if(PANS_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
if(PANS_BUILD_TOOLS)
    add_subdirectory(tools)
endif()




# 添加可执行文件（源文件列表）
# add_executable(${PROJECT_NAME}
#     pans/src/main.cpp
#     # 如果有其他源文件，在这里添加
#     # src/other.cpp
# )

# 如果项目有头文件目录，可以设置包含路径
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 可选：链接库（例如 threads）
# find_package(Threads REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

# 可选：安装规则
# install(TARGETS ${PROJECT_NAME} DESTINATION bin)