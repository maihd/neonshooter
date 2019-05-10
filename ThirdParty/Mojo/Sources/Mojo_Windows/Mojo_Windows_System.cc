#include <Mojo/System.h>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

//#include <WinSock2.h>
//#pragma comment(lib, "ws2_32.lib")

inline namespace Mojo
{
    namespace System
    {
        int CpuCount(void)
        {
            SYSTEM_INFO system_info;
            GetSystemInfo(&system_info);
            return (int)system_info.dwNumberOfProcessors;
        }

        void Sleep(int milliseconds)
        {
            ::Sleep(milliseconds);
        }

        //void MicroSleep(long long microseconds)
        //{
        //    /* 'NTSTATUS NTAPI NtDelayExecution(BOOL Alerted, PLARGE_INTEGER time);' */
        //    /* 'typedef LONG NTSTATUS;' =)) */
        //    /* '#define NTAPI __stdcall' =)) */
        //    typedef LONG(__stdcall * NtDelayExecutionFN)(BOOL, PLARGE_INTEGER);
        //    
        //    static int done_finding;
        //    static NtDelayExecutionFN NtDelayExecution;
        //    
        //    if (!NtDelayExecution && !done_finding)
        //    {
        //        done_finding = 1;
        //        HMODULE module = GetModuleHandle(TEXT("ntdll.dll"));
        //        const char* func = "NtDelayExecution";
        //        NtDelayExecution = (NtDelayExecutionFN)GetProcAddress(module, func);
        //    }
        //    
        //    if (NtDelayExecution)
        //    {
        //        LARGE_INTEGER times;
        //        times.QuadPart = -microseconds * 10;
        //        NtDelayExecution(FALSE, &times);
        //    }
        //    else
        //    {
        //        ::Sleep(microseconds / (1000));
        //    }
//
        //    //static bool init = false;
        //    //if (!init)
        //    //{
        //    //    WSADATA wsaData = {};
        //    //    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        //    //    {
        //    //        return;
        //    //    }
        //    //
        //    //    init = true;
        //    //}
        //    //
        //    //struct timeval timeOut;
        //    //timeOut.tv_sec  = 0;
        //    //timeOut.tv_usec = microseconds;
        //    //::select(0, NULL, NULL, NULL, &timeOut);
        //}

        long long PerformanceCounter(void)
        {
            LARGE_INTEGER value;
            return QueryPerformanceCounter(&value) ? value.QuadPart : 0;
        }

        long long PerformanceFrequency(void)
        {
            static long long savedValue;
            if (savedValue > 0)
            {
                return savedValue;
            }

            LARGE_INTEGER value;
            return QueryPerformanceFrequency(&value) ? (savedValue = value.QuadPart) : 0;
        }
    }
}