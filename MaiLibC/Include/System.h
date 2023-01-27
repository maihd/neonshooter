#pragma once

#ifdef _MSC_VER
#define NORETURN __declspec(noreturn)
#else
#define NORETURN __attribute__((noreturn))
#endif

#ifndef NDEBUG
#define SystemError(message, ...) SystemErrorDebug(__FUNCTION__, __FILE__, __LINE__, message, ##__VA_ARGS__)

NORETURN
void SystemErrorDebug(const char* func, const char* file, int line, const char* message, ...);

void SystemPrintAssert(const char* test, const char* func, const char* file, int line, const char* message, ...);

#if defined(_MSC_VER)
#define SystemAssert(test, message, ...)                                                            \
    do {                                                                                            \
        if (!(test)) {                                                                              \
            SystemPrintAssert(#test, __FUNCTION__, __FILE__, __LINE__, message, ##__VA_ARGS__);     \
            __debugbreak();                                                                         \
        }                                                                                           \
    } while (0)
#else
#define SystemAssert(test, message, ...)                                                            \
    do {                                                                                            \
        if (!(test)) {                                                                              \
            SystemPrintAssert(#test, __FUNCTION__, __FILE__, __LINE__, message, ##__VA_ARGS__);     \
        }                                                                                           \
    } while (0)
#endif
// ENDOF defined(_MSC_VER)

#else // ELSE OF NDEBUG
#define SystemAssert(test, message, ...) ((void)0)

NORETURN
void SystemError(const char* message, ...);
#endif
