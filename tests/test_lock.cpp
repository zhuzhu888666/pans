#include <atomic>
#include <charconv>
#include <chrono>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <latch>
#include <mutex>
#include <numeric>
#include <shared_mutex>
#include <stdexcept>
#include <string_view>
#include <thread>
#include <vector>

#include <pans/macros.h>

constexpr int RUNS = 7;

uint64_t g_target=0;
uint64_t g_counter=0;
std::atomic<uint64_t> g_atomicCounter{0};
std::mutex g_mutex;
std::shared_mutex g_sharedMutex;


void PrintResult(std::string_view name,double time)
{
    const double ops_per_second = g_target / (time / 1000.0);
    std::cout << std::left<<std::setw(14)<<name<<std::right
              <<std::setw(10)<<time<<" ms, "<<std::setw(10)<<ops_per_second<<" ops/s"<<std::endl;
}

template<typename F>
double Run(F&& f)
{
    //跑RUNS次，取平均值
    long double total_nanoseconds=0.0L;
    for(int i=0;i<RUNS;++i)
    {
        auto start=std::chrono::high_resolution_clock::now();
        f();
        auto end=std::chrono::high_resolution_clock::now();
        total_nanoseconds += std::chrono::duration<double,std::milli>(end-start).count();
    }
    return total_nanoseconds / RUNS;
}

//多线程
template<typename Operation>
std::chrono::nanoseconds BenchmarkThreads(size_t threadCount, Operation&& operation)
{
    std::latch startLatch(threadCount);
    std::latch endLatch(threadCount);
    auto startTime = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < threadCount; ++i)
    {
        std::thread([&]() {
            startLatch.count_down();
            startLatch.wait();
            operation();
            endLatch.count_down();
        }).detach();
    }
    endLatch.wait();
    auto endTime = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);
}




std::chrono::nanoseconds RunAtomic(size_t threadCount){
    g_atomicCounter=0;
    return BenchmarkThreads(threadCount, []() {
        for (uint64_t i = 0; i < g_target; ++i){
            ++g_atomicCounter;
        }
    });
}

std::chrono::nanoseconds RunMutex(size_t threadCount){
    g_counter=0;
    return BenchmarkThreads(threadCount, []() {
        for (uint64_t i = 0; i < g_target; ++i){
            std::lock_guard<std::mutex> lock(g_mutex);
            ++g_counter;
        }
    });
}
std::chrono::nanoseconds RunSharedMutex(size_t threadCount){
    g_counter=0;
    return BenchmarkThreads(threadCount, []() {
        for (uint64_t i = 0; i < g_target; ++i){
            std::unique_lock<std::shared_mutex> lock(g_sharedMutex);
            ++g_counter;
        }
    });
}



int main(int argc,char** argv)
{
    if(argc<2)
    {
        std::cout<<"Usage: "<<argv[0]<<" <target>"<<std::endl;
        return 1;
    }
    g_target=std::atoi(argv[1]);
    std::cout<<std::fixed<<std::setprecision(2);
    std::cout<<"operations: "<<g_target<<", average of "<<RUNS<<" operations"<<std::endl;
    const std::vector<size_t> threadCounts{1,2,4,8,10};
    for(const std::size_t threadCount:threadCounts)
    {
        std::cout<<"threads: "<<threadCount<<std::endl;
        for(int i=0;i<RUNS;++i)
        {
            PrintResult("atomic",Run([&](){return RunAtomic(threadCount);}));
            PrintResult("mutex",Run([&](){return RunMutex(threadCount);}));
            PrintResult("shared_mutex",Run([&](){return RunSharedMutex(threadCount);}));
            std::cout<<"\n";
        }
    }

    return 0;
}


