file(GLOB PANS_TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cxx"
)
# 创建测试可执行文件
#add_executable(test_lock ${PANS_TEST_SOURCES})
#排序
list(SORT PANS_TEST_SOURCES)
#多线程
find_package(Threads REQUIRED)

#设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

foreach(TEST_SOURCE IN LISTS PANS_TEST_SOURCES)
    message(STATUS "===========${TEST_SOURCE}===========")
    # 获取文件名（不带路径和扩展名）
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    message(STATUS ">>>>>>>>>>${TEST_NAME}<<<<<<<<<<")
    
    # 创建测试可执行文件
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    
    # 链接被测库（假设库叫 pans）
    target_link_libraries(${TEST_NAME} PRIVATE pans Threads::Threads)
    
    # 注册为 CTest 测试
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    
endforeach()


add_subdirectory(inner_tests)