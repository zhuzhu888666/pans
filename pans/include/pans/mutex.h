#ifndef PANS_INCLUDE_MUTEX_H
#define PANS_INCLUDE_MUTEX_H

#include<atomic>
#include<mutex>

#if defined(__x86_64__) || defined(__i386__)
#include<immintrin.h>

#elif defined(__aarch64__) || defined(__arm__)
#include<arm_acle.h>
#endif 

#if defined(__linux__) || defined(__GLIBC__)
#include<pthread.h>
#endif

inline void cpu_relax() {
#if defined(__x86_64__) || defined(__i386__)
    _mm_pause();
#elif defined(__aarch64__) || defined(__arm__)
    __yield();
#endif
}

namespace pans{

#if defined(__linux__) || defined(__GLIBC__)

class SpinLock {
public:
    SpinLock() noexcept{
        pthread_spin_init(&m_lock, PTHREAD_PROCESS_PRIVATE);
    };
    ~SpinLock() noexcept{
        pthread_spin_destroy(&m_lock);
    };
    SpinLock(const SpinLock&) = delete;
    SpinLock& operator=(const SpinLock&) = delete;

    void lock() noexcept{
        pthread_spin_lock(&m_lock);
    };
    void unlock() noexcept{
        pthread_spin_unlock(&m_lock);
    };
private:
    pthread_spinlock_t m_lock{};
};

#else
class SpinLock {
public:
    SpinLock() noexcept = default;
    ~SpinLock() noexcept = default;
    SpinLock(const SpinLock&) = delete;
    SpinLock& operator=(const SpinLock&) = delete;

    void lock() noexcept {
        while (m_flag.test_and_set(std::memory_order_acquire)) {
            while(m_flag.test(std::memory_order_relaxed)) {
                cpu_relax();
            }
        }
    }
    [[nodiscard]] bool try_lock() noexcept {
        return !m_flag.test_and_set(std::memory_order_acquire);
    }
    void unlock() noexcept {
        m_flag.clear(std::memory_order_release);
    }
private:
    std::atomic_flag m_flag = ATOMIC_FLAG_INIT;
};

#endif
}


#endif // PANS_INCLUDE_MUTEX_H