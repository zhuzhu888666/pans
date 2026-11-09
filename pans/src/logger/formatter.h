#ifndef PANS_SRC_LOGGER_FORMATTER_H
#define PANS_SRC_LOGGER_FORMATTER_H

#include<memory>
#include<string>
#include<vector>

#include"buffer.h"
#include"buffer_config.h"
#include"log_record.h"

namespace pans::detail{
    using ForMattedRecordBuffer=InlineBuffer<FORMATTED_RECORD_INLINE_CAPACITY>;
    class Formatter final{
        public:
            class FormatItem{
                public:
                    virtual ~FormatItem()=default;
                    virtual void format(const LogRecordView& record,ForMattedRecordBuffer& output) const noexcept=0;
            };
            explicit Formatter(std::string_view pattern);
            void format(const LogRecordView& record,ForMattedRecordBuffer& output) const noexcept;
            [[nodiscard]] const std::string& getPattern() const noexcept {return m_pattern;}
        private:
            int parse() noexcept;
            void addLiteral(std::string& literal) noexcept;
        private:
            std::string m_pattern="";
            std::vector<std::unique_ptr<FormatItem>> m_items;

    };
}

#endif