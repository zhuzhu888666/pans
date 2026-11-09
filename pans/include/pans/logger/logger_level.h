#ifndef PANS_INCLUDE_LOGGER_LEVEL_H
#define PANS_INCLUDE_LOGGER_LEVEL_H

#include<cstdint>
#include<string_view>
#include "pans/export.h"


namespace pans {
class PANS_API LoggerLevel final {
public:
    enum class Level : uint8_t {
        LOG_LV_DEBUG=1,
        LOG_LV_INFO=2,
        LOG_LV_WARN=3,
        LOG_LV_ERROR=4,
        LOG_LV_FATAL=5,
        LOG_LV_OFF=6,
    };
    /// @brief 将日志级别转换为字符串
    [[nodiscard]] static std::string_view LevelToString(Level level) noexcept;
    /// @brief 将字符串转换为日志级别
    [[nodiscard]] static Level StringToLevel(std::string_view levelStr) noexcept;
};

}

#endif // PANS_INCLUDE_LOGGER_LEVEL_H