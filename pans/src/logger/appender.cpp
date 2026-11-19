#include"logger/appender_impl.h"
#include<cerrno>
#include<iostream>
#include<stdexcept>
#include<system_error>
#include<utility>

#include"pans/macros.h"

#if defined(_WIN32)
#include<io.h>
#else
#include<unistd.h>

#endif

namespace pans{
    Appender::Appender(std::unique_ptr<Impl> impl) noexcept :m_impl(std::move(impl)){};
    void Appender::setLevel(LogLevel::Level level) noexcept{
        m_impl->setLevel(level);
    }
    void Appender::flush() {
        m_impl->flush();
    };
    void Appender::sync(){
        m_impl->sync();
    };
    void Appender::Impl::append(LogLevel::Level level,std::string_view formatted_record) noexcept{
        if(static_cast<std::uint8_t> (level)< static_cast<std::uint8_t>(getLevel())){
            return;
        }
        try
        {
            std::lock_guard<std::mutex> lock(this->m_mutex);
            writeUnlocked(formatted_record);
            if(level==LogLevel::Level::LOG_LV_FATAL){
               flushUnlocked();
            }

        }
        catch(...)
        {
            constexpr std::string_view message="pans logger: appender operation failed.\n";
            std::fwrite(message.data(),1,message.size(),stderr);
        }
    }
    void Appender::Impl::setLevel(LogLevel::Level level) noexcept{
        m_level.store(level,std::memory_order_release);
    }
    LogLevel::Level Appender::Impl::getLevel() const noexcept{
        return m_level.load(std::memory_order_acquire);
    }
    void Appender::Impl::flush(){
        std::lock_guard<std::mutex> lock(m_mutex);
        flushUnlocked();
    }
    void Appender::Impl::sync(){
        std::lock_guard<std::mutex> lock(m_mutex);
        syncUnlocked();
    }
};

