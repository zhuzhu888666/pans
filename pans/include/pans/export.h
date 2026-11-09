#ifndef PANS_INCLUDE_EXPORT_H
#define PANS_INCLUDE_EXPORT_H


#if defined(_WIN32) && defined(PANS_SHARED_LIB)
    #if defined(PANS_BUILDING_LIBRARY)
        #define PANS_API __declspec(dllexport)
    #else
        #define PANS_API __declspec(dllimport)
    #endif
#elif defined(__GNUC__) && defined(PANS_SHARED_LIB)
    #define PANS_API __attribute__((visibility("default")))
#else
    #define PANS_API
#endif



#endif // PANS_INCLUDE_EXPORT_H