#ifndef PANS_INCLUDE_MACROS_H
#define PANS_INCLUDE_MACROS_H

#include<cassert>
#include<iostream>
#include<utility>
#include<cstdint>


#define PANS_ASSERT(x) \
    do { \
        if (!(x))[[unlikely]] { \
            std::cerr << "Assertion failed: " << #x << ", file " << __FILE__ << ", line " << __LINE__ << std::endl; \
            assert(x); \
        } \
    } while (0)

//
#define PANS_ASSERT_MSG(x, msg) \
    do { \
        if (!(x))[[unlikely]] { \
            std::cerr << __FILE__<<":"<<__LINE__<<"Assert "<<#x<<" Failed.[" << msg << "].\n" << std::endl; \
            assert(x); \
        } \
    } while (0)

#define ASSERT_RETVAL(x, ret) \
    do { \
        if (x)[[likely]] break; \
        PANS_ASSERT(x); \
        return ret; \
    } while (0)

#define ASSERT_RETVAL2(x, ret, msg) \
    do { \
        if (x)[[likely]] break; \
        PANS_ASSERT_MSG(x, msg); \
        return ret; \
    } while (0)

//无返回值
#define ASSERT_RETNONE(x) \
    do { \
        if (x)[[likely]] break; \
        PANS_ASSERT(x); \
        return; \
    } while (0)

#define ASSERT_RETNONE2(x, msg) \
    do { \
        if (x)[[likely]] break; \
        PANS_ASSERT_MSG(x, msg); \
        return; \
    } while (0)


#endif // PANS_INCLUDE_MACROS_H