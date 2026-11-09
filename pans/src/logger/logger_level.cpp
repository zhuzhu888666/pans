#include "pans/logger/logger_level.h"
#include<algorithm>
#include<array>
#include<cctype>
#include<string>

namespace pans {

    std::string_view LoggerLevel::LevelToString(Level level) noexcept {
        // switch (level) {
        //     case Level::LOG_LV_DEBUG: return "DEBUG";
        //     case Level::LOG_LV_INFO: return "INFO";
        //     case Level::LOG_LV_WARN: return "WARN";
        //     case Level::LOG_LV_ERROR: return "ERROR";
        //     case Level::LOG_LV_FATAL: return "FATAL";
        //     case Level::LOG_LV_OFF: return "OFF";
        //     default: return "UNKNOWN";
        // };
        switch(level){
        #define XX(name) case Level::LOG_LV_##name: return #name;
            XX(DEBUG)
            XX(INFO)
            XX(WARN)
            XX(ERROR)
            XX(FATAL)
            XX(OFF)
        #undef XX
            default: return "UNKNOWN";
        };
        return "UNKNOWN";
    };
    LoggerLevel::Level LoggerLevel::StringToLevel(std::string_view levelStr) noexcept {
        std::array<char,6> normalizedStr{};
        // 将输入字符串转换为大写形式
        std::transform(levelStr.begin(), levelStr.end(), normalizedStr.begin(), [](char c) { return std::toupper(static_cast<unsigned char>(c)); });
        // 创建一个 std::string_view 来表示大写后的字符串
        std::string_view upper_value(normalizedStr.data(), levelStr.size());
        // 使用宏来简化代码
        #define XX(name) if(upper_value==#name) return Level::LOG_LV_##name;
            XX(DEBUG)
            XX(INFO)
            XX(WARN)
            XX(ERROR)
            XX(FATAL)
            XX(OFF)
        #undef XX
        return Level::LOG_LV_OFF; // 默认返回 OFF

        // if (upper_value == "DEBUG") return Level::LOG_LV_DEBUG;
        // if (upper_value == "INFO") return Level::LOG_LV_INFO;
        // if (upper_value == "WARN") return Level::LOG_LV_WARN;
        // if (upper_value == "ERROR") return Level::LOG_LV_ERROR;
        // if (upper_value == "FATAL") return Level::LOG_LV_FATAL;
        // if (upper_value == "OFF") return Level::LOG_LV_OFF;
        // return Level::LOG_LV_OFF; // 默认返回 OFF
    };


} // namespace pans