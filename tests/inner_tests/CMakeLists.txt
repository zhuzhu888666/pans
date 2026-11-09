set(INNER_TEST_NAME test_log_buffer)
set(INNER_TEST_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/test_log_buffer.cpp")

add_executable(${INNER_TEST_NAME} ${INNER_TEST_SOURCE})

target_link_libraries(${INNER_TEST_NAME} PRIVATE pans Threads::Threads)
target_include_directories(${INNER_TEST_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/pans/src")
#设置输出目录
set_target_properties(${INNER_TEST_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

add_test(NAME ${INNER_TEST_NAME} COMMAND ${INNER_TEST_NAME})