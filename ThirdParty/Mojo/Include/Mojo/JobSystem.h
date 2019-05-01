#pragma once

inline namespace Mojo
{
    struct Allocator;
    struct JobCounter;

    using JobEntry = void(*)(void*);
    
    struct Job
    {
        JobEntry    _entry;
        void*       _args;
        JobCounter* _counter;

        Job() 
            : _entry(0)
            , _args(0)
            , _counter(0)
        {
        }

        Job(JobEntry entry, void* args)
            : _entry(entry)
            , _args(args)
            , _counter(0)
        {
        }
    };

    namespace JobSystem
    {
        bool        Setup(int maxJobsPerThread = 256, Allocator* allocator = 0);
        void        Shutdown(void);

        void        Execute(const Job& job, JobCounter* counter = 0);
        void        Execute(const Job* jobs, int count, JobCounter* counter = 0);

        void        WaitCounter(JobCounter* counter);
    }
}