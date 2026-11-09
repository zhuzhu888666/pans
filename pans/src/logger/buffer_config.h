#ifndef PANS_INCLUDE_LOGGER_BUFFER_CONFIG_H
#define PANS_INCLUDE_LOGGER_BUFFER_CONFIG_H

#include<cstddef>

namespace pans::detail  {
    inline constexpr std::size_t LOG_MESSAGE_INLINE_CAPACITY = 128; // 日志消息的内联容量
    inline constexpr std::size_t PRINTF_FORMAT_INLINE_CAPACITY = 128; // printf 格式化字符串的内联容量
    inline constexpr std::size_t FORMATTED_RECORD_INLINE_CAPACITY = 256; // 格式化后的日志记录的内联容量
    
}


#endif //PANS_INCLUDE_LOGGER_BUFFER_CONFIG_H