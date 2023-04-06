namespace Mojo;

using SDL2;
using System;

public static class System
{
	[CallingConvention(.Stdcall)]
    function int32 NtDelayExecutionFN(int alerted, int64* time);

	[LinkName("GetModuleHandleA"), CallingConvention(.Stdcall)]
	static extern void* GetModuleHandle(char8* name);
	
	[LinkName("GetProcAddress"), CallingConvention(.Stdcall)]
	static extern void* GetProcAddress(void* module, char8* name);

	public static int32 CpuCount()
	{
	    return SDL.GetCPUCount();
	}

	public static void Sleep(int milliseconds)
	{
	    SDL.Delay((.)milliseconds);
	}

	public static void MicroSleep(int64 microseconds)
	{
#if BF_PLATFORM_WINDOWS
	    /* 'NTSTATUS NTAPI NtDelayExecution(BOOL Alerted, PLARGE_INTEGER time);' */
	    /* 'typedef LONG NTSTATUS;' =)) */
	    /* '#define NTAPI __stdcall' =)) */
	    static bool 				done_finding	 = false;
	    static NtDelayExecutionFN 	NtDelayExecution = null;

	    if (NtDelayExecution == null && !done_finding)
	    {
	        done_finding = true;

	        let module = GetModuleHandle("ntdll.dll");
	        NtDelayExecution = (NtDelayExecutionFN)GetProcAddress(module, "NtDelayExecution");
	    }
	    
	    if (NtDelayExecution != null)
	    {
	        int64 times = -microseconds * 10;
	        NtDelayExecution(0, &times);
	    }
	    else
	    {
	        Sleep(microseconds / (1000));
	    }
#else
	    usleep((useconds_t)microseconds);
#endif
	}

	public static int64 PerformanceCounter()
	{
	    return (.)SDL.GetPerformanceCounter();
	}

	public static int64 PerformanceFrequency()
	{
	    static int64 savedValue = 0;
	    if (savedValue > 0)
	    {
	        return savedValue;
	    }

	    return (savedValue = (.)SDL.GetPerformanceFrequency());
	}
}