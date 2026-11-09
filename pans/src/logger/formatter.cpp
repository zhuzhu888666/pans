#include"formatter.h"
#include"pans/macros.h"
#include<unordered_map>

namespace pans::detail{
    constexpr std::string_view DEFAULT_DATA_FOMAT="%Y-%m-%d %H:%M:%S";
    Formatter::Formatter(std::string_view pattern):m_pattern(pattern){
        if(parse()){throw std::invalid_argument("invalid log format,parse failed.");}
    };
    void Formatter::format(const LogRecordView& record,ForMattedRecordBuffer& output) const noexcept{
        for(const auto& item:m_items){
            item->format(record,output);
        }
    }
    template<typename T>
    void AppendInteger(ForMattedRecordBuffer& output,T value) noexcept{
        std::array<char,24> buffer{};
        const auto result=std::to_chars(buffer.data(),buffer.data()+buffer.size(),value);
        ASSERT_RETNONE2(result.ec==std::errc(),"trans "<<value<<"to chars failed.");
        output.append(buffer.data(),static_cast<std::size_t>(result.ptr-buffer.data()));
    }
    class LiteralFormatItem final: public Formatter::FormatItem{
        public:
            explicit LiteralFormatItem(std::string value):m_value(value){};
            void format(const LogRecordView& record,ForMattedRecordBuffer& output)const noexcept{ 
                output.append(m_value);
            };
        private:
            std::string m_value;
    };
    class MessageFormatItem final: public Formatter::FormatItem{
        public:
            void format(const LogRecordView& record,ForMattedRecordBuffer& output) const noexcept {
                output.append(record.m_message);
            };
    };
    class LevelFormatItem final:public Formatter::FormatItem{
        public:
            void format(const LogRecordView& record,ForMattedRecordBuffer& output) const noexcept{
                output.append(LoggerLevel::LevelToString(record.m_level));
            }
    };
    class ElapsedFormatItem final: Formatter::FormatItem{
        public:
            void format(const LogRecordView& record,ForMattedRecordBuffer& output) const noexcept{
                AppendInteger(output,std::chrono::duration_cast<std::chrono::milliseconds>(record.m_elapsed).count());
            }
    };
    class LoggerNameFormatItem final: public Formatter::FormatItem{
        public:
            void format(const LogRecordView& record,ForMattedRecordBuffer& output) const noexcept{
                output.append(record.m_loggerName);
            }
    };
    class ThreadIdFormatItem final: public Formatter::FormatItem{
        public:
            void format(const LogRecordView& record,ForMattedRecordBuffer& output) const noexcept{
                AppendInteger(output,record.m_threadId);
            }
    };
    class NewLineFormatItem final: public Formatter::FormatItem{
        void format(const LogRecordView& record,ForMattedRecordBuffer& output) const noexcept{
            output.append('\n');
        }
    };
}