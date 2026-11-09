#ifndef PANS_INCLUDE_LOGGER_BUFFER_H
#define PANS_INCLUDE_LOGGER_BUFFER_H


#include<algorithm>
#include<cstddef>
#include<array>
#include<cstring>
#include<streambuf>
#include<string_view>
#include<vector>

namespace pans::detail {

template<std::size_t INLINE_CAPACITY>
class InlineBuffer {
public:
    InlineBuffer() = default;
    void append(const char* data,std::size_t size){
        if(size==0) return;
        if(m_overflow.empty() && m_size + size <= INLINE_CAPACITY){
            // 栈上缓冲区有足够的空间，直接写入栈
            std::memcpy(m_inlineBuffer.data() + m_size, data, size);
            m_size += size;
        }
        if(m_overflow.empty()){
            // 栈上缓冲区已满，切换到堆上缓冲区
            // 计算新的容量，至少为当前大小的两倍，或者足够容纳新数据
            const std::size_t newCapacity = std::max(INLINE_CAPACITY * 2, m_size + size);
            m_overflow.reserve(newCapacity);
            // 将栈上的数据复制到堆区
            m_overflow.insert(m_overflow.end(), m_inlineBuffer.data(), m_inlineBuffer.data() + m_size);
        }
        // 将数据写入堆上缓冲区
        m_overflow.insert(m_overflow.end(), data, data + size);
        m_size=m_overflow.size();
    };
    void append(std::string_view str){
        append(str.data(),str.size());
    };
    void append(char c){
        append(&c,1);
    };
    [[nodiscard]] const char* data() const noexcept{
        return m_overflow.empty() ? m_inlineBuffer.data() : m_overflow.data();
    };
    [[nodiscard]] std::size_t size() const noexcept{
        return m_size;
    };
    [[nodiscard]] std::string_view view() const noexcept{
        return {data(),size()};
    };
private:
    // 内联缓冲区的容量
    std::array<char, INLINE_CAPACITY> m_inlineBuffer{};
    std::vector<char> m_overflow{};
    //日志长度
    std::size_t m_size{0};
};

template<std::size_t INLINE_CAPACITY>
class SmallStreamBuffer final:public std::streambuf{
    public:
        // 构造函数，接受一个InlineBuffer引用
        explicit SmallStreamBuffer(InlineBuffer<INLINE_CAPACITY>& buffer) : m_buffer(buffer) {};
    protected:
        std::streamsize xsputn(const char* data, std::streamsize n) override {
            if(n == 0) return 0;
            m_buffer.append(data, static_cast<std::size_t>(n));
            return n;
        };
        // 处理单个字符的写入
        int_type overflow(int_type ch) override {
            // 如果写入的字符是EOF，返回not_eof
           if(traits_type::eq_int_type(ch, traits_type::eof())) {
               return traits_type::not_eof(ch);
           }
           m_buffer.append(traits_type::to_char_type(ch));
           return ch;
        };
    private:
        InlineBuffer<INLINE_CAPACITY>& m_buffer;
};


}



#endif // PANS_INCLUDE_LOGGER_BUFFER_CONFIG_H