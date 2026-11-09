#ifndef PANS_SRC_LOGGER_LOG_RECORD_H
#define PANS_SRC_LOGGER_LOG_RECORD_H
#include<chrono>
#include<stdint.h>
#include<source_location>
#include<string_view>
#include "pans/logger/logger_level.h"


namespace pans::detail{
    struct LogRecordView
    {
        LoggerLevel::Level m_level=LoggerLevel::Level::LOG_LV_DEBUG;
        std::string_view m_loggerName="";//日志器名字
        std::string_view m_message="";//日志内容
        std::chrono::system_clock::time_point m_timestamp;//时间戳
        std::chrono::steady_clock::duration m_elapsed;//起服到现在已经经过的时间
        std::uint64_t m_threadId=0;//线程id
        std::uint64_t m_fiberId=0;//协程id
        std::string_view m_threadName="";//线程名
        std::string_view m_fileName="";//文件名
        std::string_view m_functionName="";//函数名
        std::source_location m_location;
    };
    
}

#endif // PANS_SRC_LOGGER_LOG_RECORD_H
