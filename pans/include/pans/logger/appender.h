#ifndef PANS_INCLUDE_PANS_LOGGER_APPENDER_H
#define PANS_INCLUDE_PANS_LOGGER_APPENDER_H

#include<memory>
#include<string>

#include"pans/export.h"
#include"pans/logger/logger_level.h"

namespace pans{

namespace detail{
    class AppenderAccess;
};
class PANS_API Appender final{
    public:
        class Impl;
        ~Appender()=default;
        Appender(const Appender&)=delete;
        Appender& operator=(const Appender&)=delete;
        Appender(Appender&&)=delete;
        Appender& operator=(Appender&&)=delete;

        void setLevel(LogLevel::Level level) noexcept;
        [[nodiscard]] LogLevel::Level getLevel() noexcept;

        void flush();
        void sync();
    private:
        explicit Appender(std::unique_ptr<Impl> Impl) noexcept;//
        std::unique_ptr<Impl> m_impl;
        friend class detail::AppenderAccess;
};
    using AppenderPtr=std::shared_ptr<Appender>;
    [[nodiscard]] PANS_API AppenderPtr MakeStdoutAppender();
    [[nodiscard]] PANS_API AppenderPtr MakeFIleAppender(std:: string fileName);
    

};

#endif