#include <Mojo/Core/Array.h>
#include <Mojo/Core/Atomic.h>
#include <Mojo/Core/Thread.h>
#include <Mojo/Core/Coroutine.h>

#include <Mojo/JobSystem.h>
#include <Mojo/JobCounter.h>
#include <Mojo/Native/System.h>

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <Windows.h>

inline void* operator new (decltype(sizeof(void*)), const decltype(nullptr)&, void* pointer)
{
    return pointer;
}

inline void operator delete (void*, const decltype(nullptr)&, void*)
{
}

namespace Mojo
{
    bool JobCounter_IsDone(JobCounter* counter)
    {
        return counter->_count.value <= 0;
    }

    void JobCounter_OneJobDone(JobCounter* counter)
    {
        counter->_count--;
        if (JobCounter_IsDone(counter))
        {
            counter->_signal.Signal();
        }
    }

    void JobCounter_Wait(JobCounter* counter)
    {
        counter->_signal.Wait(counter->_mutex);
    }
    
    struct JobQueue
    {
        int   head;
        int   tail;
        int   size;
        Job*  data;

        inline int Push(const Job& item)
        {
            int index = -1;
            int next = (head + 1) % size;
            if (next != tail)
            {
                index = head;
                data[index] = item;

                head = next;
            }

            return index;
        }

        inline int Pop(Job* item)
        {
            int index = -1;
            if (tail != head)
            {
                index = tail;
                *item = data[index];

                tail = (tail + 1) % size;
            }

            return index;
        }

        int GetCount(void)
        {
            return tail > head ? tail - head : head - tail;
        }
    };

    inline namespace JobSystem_Variables
    {
        namespace Flags
        {
            enum
            {
                None,
                Init    = 1 << 0,
                Running = 1 << 1,
            };
        };

        static int                  _flags;

        static int                  _maxJobsPerThread;
        static int                  _workerThreadCount;

        static ThreadMutex          _wakeMutex;
        static ThreadSignal         _wakeSignal;

        static JobQueue*            _jobQueues;
        static int                  _jobQueueIndex;

        //static Coroutine*           _coroutinePools;

        static AtomicI32         _currentCounter;
        static AtomicI32         _finishedCounter;

        static Allocator*           allocator;
    }

    static void JobSystem_CoroutineEntry(void* args)
    {
        Job* job = (Job*)args;
        if (job->_entry)
        {
            job->_entry(job->_args);
        }
    }

    static int JobSystem_ThreadEntry(void* args)
    {
        int threadIndex = (int)(intptr_t)args;
        JobQueue&  jobQueue      = _jobQueues[threadIndex];
        //Coroutine* coroutinePool = &_coroutinePools[threadIndex * _maxJobsPerThread];

        while (_flags & Flags::Running)
        {
            Job job;
            int index;
            if ((index = jobQueue.Pop(&job)) != -1)
            {
                //Coroutine& coroutine = coroutinePool[index];
                //if (!coroutine.Status())
                //{
                //    if (!coroutine.Start(JobSystem_CoroutineEntry, &jobQueue.data[index]))
                //    {
                //        assert(false && "System internal error.");
                //    }
                //}

                //coroutine.Resume();
                //if (coroutine.Status())
                //{
                //    // The job is not finish yet
                //    _currentCounter++;
                //    jobQueue.Push(job);
                //}
                //else
                //{
                //    if (job._counter)
                //    {
                //        JobCounter_OneJobDone(job._counter);
                //    }
                //}
                
                job._entry(job._args);
                if (job._counter)
                {
                    JobCounter_OneJobDone(job._counter);
                }

                // update worker label state
                _finishedCounter++;
            }
            else
            {
                // Sleep thread to save cpu usage
                _wakeSignal.Wait(_wakeMutex);
            }
        }

        return 0;
    }

    namespace JobSystem
    {
        bool Init(int maxJobsPerThread, Allocator* allocator)
        {
            // Prevent re-init
            if (_flags & Flags::Init)
            {
                return true;
            }

            // We cannot initialize if job pool have no size
            if (maxJobsPerThread <= 0)
            {
                return false;
            }

            if (!allocator)
            {
                allocator = Allocator::Default;
            }

            int cpuCores = System::CpuCores();

            _flags |= Flags::Init;
            _flags |= Flags::Running;

            allocator = allocator;

            _maxJobsPerThread  = maxJobsPerThread;
            _workerThreadCount = cpuCores;

            _jobQueues      = (JobQueue*)allocator->Acquire(sizeof(JobQueue) * cpuCores + cpuCores * maxJobsPerThread * sizeof(Job), alignof(Job));
            _jobQueueIndex  = 0;
            //_coroutinePools = (Coroutine*)allocator->Acquire(cpuCores * maxJobsPerThread * sizeof(Coroutine), alignof(Coroutine));

            _currentCounter.value  = 0;
            _finishedCounter.value = 0;

            //::memset(_coroutinePools, 0, cpuCores * maxJobsPerThread * sizeof(Coroutine));

            for (int i = 0; i < _workerThreadCount; i++)
            {
                JobQueue& jobQueue = _jobQueues[i];
                jobQueue.head = 0;
                jobQueue.tail = 0;
                jobQueue.size = maxJobsPerThread;
                jobQueue.data = &((Job*)(_jobQueues + cpuCores))[i * maxJobsPerThread];

                Thread thread;
                if (!Thread::Run(JobSystem_ThreadEntry, (void*)(intptr_t)i))
                {
                    JobSystem::Shutdown(); // Should call to save memory
                    return false;
                }
            }

            return true;
        }

        void Execute(const Job& job, JobCounter* counter)
        {
            // Prevent do something weird
            if ((_flags & Flags::Running) == 0)
            {
                return;
            }

            _currentCounter++;

            int index;
            while ((index = _jobQueues[_jobQueueIndex].Push(job)) == -1)
            {
                _wakeSignal.Signal();
            }

            _jobQueues[_jobQueueIndex].data[index]._counter = counter;
            _jobQueueIndex = (_jobQueueIndex + 1) % _workerThreadCount;

            _wakeSignal.Signal();
        }

        void Execute(const Job* jobs, int count, JobCounter* counter)
        {
            // Prevent do something weird
            if ((_flags & Flags::Running) == 0)
            {
                return;
            }

            for (int i = 0; i < count; i++)
            {
                Execute(jobs[i], counter);
            }
        }

        void WaitCounter(JobCounter* counter)
        {
            if (counter)
            {
                JobCounter_Wait(counter);
            }
        }

        bool IsBusy(void)
        {
            return _finishedCounter.value != _currentCounter.value;
        }

        void Shutdown(void)
        {
            while (IsBusy())
            {
                _wakeSignal.Signal();
            }

            //allocator->Release(_coroutinePools);
            allocator->Release(_jobQueues);

            _flags = ~Flags::None;

            _maxJobsPerThread  = 0;
            _workerThreadCount = 0;

            _jobQueues      = 0;
            _jobQueueIndex  = 0;
            //_coroutinePools = 0;

            _currentCounter.value  = 0;
            _finishedCounter.value = 0;

            allocator = 0;
        }
    }
}