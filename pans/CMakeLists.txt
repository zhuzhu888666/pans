# 收集当前目录及子目录下的所有 C/C++ 源文件
# GLOB_RECURSE：递归查找；CONFIGURE_DEPENDS：源文件增删时自动重新运行 CMake 配置
file(GLOB_RECURSE PANS_SOURCES CONFIGURE_DEPENDS *.cpp *.cc *.c)
# 收集当前目录及子目录下的所有头文件（加入构建目标，便于 IDE 索引）
file(GLOB_RECURSE PANS_HEADERS CONFIGURE_DEPENDS *.h *.hpp)

# 将收集到的源文件和头文件打包成一个静态库，库名为 pans
add_library(pans STATIC ${PANS_SOURCES} ${PANS_HEADERS})

# 设置 pans 库的头文件搜索路径
target_include_directories(pans
    PUBLIC
        # 构建期（在本项目内被其他目标使用时）暴露 include 目录
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        # 安装后（被外部项目通过 find_package 使用）暴露安装目录下的 include 路径
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        # 仅在 pans 自身编译时使用的 src 目录（不对外暴露）
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# 链接 pans_options 接口库，继承其所有编译/链接选项（如警告、标准等）
target_link_libraries(pans PUBLIC pans_options)

# 为 pans 库添加私有编译选项
target_compile_options(pans PRIVATE
    # 主库可能需要更严格的警告
    # 仅在 GCC 或 Clang 编译器下生效
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
        -Werror          # 将警告视为错误（主库严格要求）
    >
)

# 安装 pans 库及其导出信息
# EXPORT pansTargets：将安装信息导出为 pansTargets，供外部项目 find_package 使用
install(TARGETS pans
    EXPORT pansTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}   # 静态库（.a）安装路径
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}   # 动态库（.so）安装路径
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}   # 可执行文件/Windows DLL 安装路径
)

# 同样安装 pans_options 接口库（无需指定目标类型），并导出到 pansTargets
install(TARGETS pans_options EXPORT pansTargets)

# 安装公共头文件目录到系统 include 目录
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
