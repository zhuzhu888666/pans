add_library(pans_options INTERFACE)

# ===== 通用警告选项（GCC/Clang）=====
target_compile_options(pans_options INTERFACE
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:
        -Wall
        -Wextra           # 额外警告（比 -Wall 更严格）
        -Wpedantic        # 严格遵循标准
        -fno-strict-aliasing
        -Wno-builtin-macro-redefined # 消除__FILE__重定义告警
    >
)

# ===== 通用警告选项（MSVC）=====
target_compile_options(pans_options INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:
        /W4                 # 高警告等级
        /permissive-        # 严格标准符合模式
        /Zc:__cplusplus     # 正确报告 __cplusplus 宏
        /utf-8              # 源码与执行字符集均为 UTF-8
        /MP                 # 多进程编译
    >
)

# ===== Linux 平台（GCC/Clang）=====
target_compile_options(pans_options INTERFACE
    $<$<AND:$<PLATFORM_ID:Linux>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:
        -fPIC       # 编译位置无关代码
    >
)

target_link_options(pans_options INTERFACE
    $<$<AND:$<PLATFORM_ID:Linux>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:
        -rdynamic         # 栈回溯需要
    >
)

# ===== Debug 配置 =====
target_compile_definitions(pans_options INTERFACE
    $<$<CONFIG:Debug>:PANS_DEBUG>
)

target_compile_options(pans_options INTERFACE
    # GCC/Clang
    $<$<AND:$<CONFIG:Debug>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:
        -O0
        -g3
        -ggdb
    >
    # MSVC
    $<$<AND:$<CONFIG:Debug>,$<CXX_COMPILER_ID:MSVC>>:
        /Od
        /Zi
        /RTC1
    >
)

# ===== Release/RelWithDebInfo 共用选项 =====
target_compile_options(pans_options INTERFACE
    # GCC/Clang Release
    $<$<AND:$<CONFIG:Release>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:
        -O2
        -fno-omit-frame-pointer
    >
    # GCC/Clang RelWithDebInfo
    $<$<AND:$<CONFIG:RelWithDebInfo>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:
        -O2
        -g
        -fno-omit-frame-pointer
    >
    # MSVC Release
    $<$<AND:$<CONFIG:Release>,$<CXX_COMPILER_ID:MSVC>>:
        /O2
    >
    # MSVC RelWithDebInfo
    $<$<AND:$<CONFIG:RelWithDebInfo>,$<CXX_COMPILER_ID:MSVC>>:
        /O2
        /Zi
    >
)

# NDEBUG 定义对所有编译器通用
target_compile_definitions(pans_options INTERFACE
    $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>>:NDEBUG>
)

# ===== 覆盖率选项（默认关闭，仅 GCC/Clang）=====
option(ENABLE_COVERAGE "Enable code coverage instrumentation" OFF)
if(ENABLE_COVERAGE)
    target_compile_options(pans_options INTERFACE
        $<$<AND:$<CONFIG:Debug>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:--coverage>
    )
    target_link_options(pans_options INTERFACE
        $<$<AND:$<CONFIG:Debug>,$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>>:--coverage>
    )
endif()