#include <Mojo/System.h>

#if _WIN32
#   define VC_EXTRALEAN
#   define WIN32_LEAN_AND_MEAN
#   include <Windows.h>
#else
#   include <unistd.h>
#endif

inline namespace Mojo
{
    namespace System
    {
        void MicroSleep(long long microseconds)
        {
#if _WIN32
            /* 'NTSTATUS NTAPI NtDelayExecution(BOOL Alerted, PLARGE_INTEGER time);' */
            /* 'typedef LONG NTSTATUS;' =)) */
            /* '#define NTAPI __stdcall' =)) */
            typedef LONG(__stdcall * NtDelayExecutionFN)(BOOL, PLARGE_INTEGER);
            
            static int done_finding;
            static NtDelayExecutionFN NtDelayExecution;
            
            if (!NtDelayExecution && !done_finding)
            {
                done_finding = 1;
                HMODULE module = GetModuleHandle(TEXT("ntdll.dll"));
                const char* func = "NtDelayExecution";
                NtDelayExecution = (NtDelayExecutionFN)GetProcAddress(module, func);
            }
            
            if (NtDelayExecution)
            {
                LARGE_INTEGER times;
                times.QuadPart = -microseconds * 10;
                NtDelayExecution(FALSE, &times);
            }
            else
            {
                ::Sleep(microseconds / (1000));
            }
#else
            usleep((useconds_t)microseconds);
#endif
        }
    }
}