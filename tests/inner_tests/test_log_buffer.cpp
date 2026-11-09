#include<array>
#include<atomic>
#include<chrono>
#include<cstddef>
#include<cstdint>

#include<iomanip> //setw

#include<iostream>
#include<sstream>
#include<string>
#include<string_view>

#include"buffer.h"
#include"buffer_config.h"


constexpr int RUNS=7;
std::uint64_t g_target=0;
std::atomic<std::uint64_t> BENCHMARK_SINK{0};

std::uint64_t Observe(std::string_view value) noexcept
{
    return static_cast<std::uint64_t>(value.size())+
    static_cast<std::uint64_t>(value.front())+
    static_cast<std::uint64_t>(value[value.size()/2])+
    static_cast<std::uint64_t>(value.back());
}

std::uint64_t WriteWithBuffer(std::string_view message) noexcept
{
    pans::detail::InlineBuffer<pans::detail::LOG_MESSAGE_INLINE_CAPACITY> buffer;
    pans::detail::SmallStreamBuffer<pans::detail::LOG_MESSAGE_INLINE_CAPACITY> streamBuffer(buffer);
    std::ostream os(&streamBuffer);
    os<<message;
    return Observe(buffer.view());
}

std::uint64_t WriteWithStringStream(std::string_view message){
    std::stringstream os;
    os<<message;
    return Observe(os.str());
}

template<typename Operation>
std::chrono::nanoseconds BenchmarkWrites(Operation& operation){
    std::uint64_t checksum=0;
    const auto begin=std::chrono::steady_clock::now();
    for(std::uint64_t iter=0;iter<g_target;iter++){
        checksum+=operation();
    }
    const auto end=std::chrono::steady_clock::now();
    BENCHMARK_SINK.fetch_xor(checksum,std::memory_order_relaxed);
    return std::chrono::duration_cast<std::chrono::nanoseconds>(end-begin);
}

template<typename F>
double Run(F& f){
    long double total_nanoseconds=0;
    for (size_t i = 0; i < RUNS; i++)
    {
        total_nanoseconds+=static_cast<long double>(f().count());
    }
    return static_cast<double>(total_nanoseconds/RUNS);
}

