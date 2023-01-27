#include <System.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef NDEBUG
NORETURN
void SystemErrorDebug(const char* func, const char* file, int line, const char* message, ...)
{
    int ret;

    ret = fprintf(stderr, "[SystemError] At %s:%d:%s\n\tReason: ", file, line, func);

    va_list vargs;
    va_start(vargs, message);
    ret = vfprintf(stderr, message, vargs);
    va_end(vargs);

    fputc('\n', stderr);
    (void)ret;

    exit(1);
}

void SystemPrintAssert(const char* test, const char* func, const char* file, int line, const char* message, ...)
{
    int ret;

    ret = fprintf(stderr, "[SystemAssert] At %s:%d:%s\n\tTest: %s\n\tReason: ", file, line, func, test);

    va_list vargs;
    va_start(vargs, message);
    ret = vfprintf(stderr, message, vargs);
    va_end(vargs);

    fputc('\n', stderr);
    (void)ret;
}
#else
NORETURN
void SystemError(const char* message, ...)
{
    int ret;

    ret = fprintf(stderr, "[System Error] ");

    va_list vargs;
    va_start(vargs, message);
    ret = vfprintf(stderr, message, vargs);
    va_end(vargs);

    fputc('\n', stderr);
    return ret;

    exit(1);
}
#endif