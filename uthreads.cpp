// ISSUES & UPDATES:
// 1) Why .Hpp?
// 2) Why Implement things in the header?
// 3) uthread_init: we shouldn't initialize the main thread, it is already exists: https://moodle2.cs.huji.ac.il/nu18/mod/forum/discuss.php?d=44535
//    The scheduler should be created when _running is already 0, and the manager should contain thread 0 from the beginning.
// 4) I massively changed scheduler: we had several problems since block, termination, sleep and timeout are different
//    cases that require different treatment, which the older API didn't allow.
// 5) I updated Terminate, block according to the new scheduler API.
// 6) Fixed bug in uthread_resume.

// TASKS:
// TODO: Protect critical code
// TODO: Implement timer mechanizem.
// TODO: Implement Switch in scheduler
// TODO: Implement Switch in manager.



#include "uthreads.h"
#include "thread_manager.h"
#include "scheduler.h"

#include <queue>
#include <unordered_map>
#include <iostream>

//-------------Error Massages:
#define LIB_ERROR_SYNTAX "thread library error: "
#define SYS_ERROR_SYNTAX "system error: "


//-------------Static Globals:
static thread_manager manager(0, MAX_THREAD_NUM, STACK_SIZE); // TODO: How to initialise it?
static scheduler scheduler;
static int totalQuants = 0;

//-------------Timer:
//TODO: TIMER DECLARATION AND SETTING & HANDLER & totalQuants maintenance



//---------------------------------Library Functionality---------------------
/*
 * Description: This function initializes the thread library.
 * You may assume that this function is called before any other thread library
 * function, and that it is called exactly once. The input to the function is
 * the length of a quantum in micro-seconds. It is an error to call this
 * function with non-positive quantum_usecs.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_init(int quantum_usecs)
{
    if (quantum_usecs >= 0)
    {
        //create global functionality holders:
        manager = thread_manager(quantum_usecs, MAX_THREAD_NUM, STACK_SIZE);
        scheduler = scheduler;

        // init timer
    }
    std::cerr << LIB_ERROR_SYNTAX << "quantum_usec should be non-negative." << std::endl;
    return -1;
}

/*
 * Description: This function creates a new thread, whose entry point is the
 * function f with the signature void f(void). The thread is added to the end
 * of the READY threads list. The uthread_spawn function should fail if it
 * would cause the number of concurrent threads to exceed the limit
 * (MAX_THREAD_NUM). Each thread should be allocated with a stack of size
 * STACK_SIZE bytes.
 * Return value: On success, return the ID of the created thread.
 * On failure, return -1.
*/
int uthread_spawn(void (*f)()){
    int newTid = manager.createThread(f);

    if(newTid != -1){
        scheduler.appendTid(newTid);
        return newTid;
    }

    std::cerr <<  LIB_ERROR_SYNTAX << "Number of threads > MAX_THREAD_NUMBER." << std::endl;
    return  -1;
}


/*
 * Description: This function terminates the thread with ID tid and deletes
 * it from all relevant control structures. All the resources allocated by
 * the library for this thread should be released. If no thread with ID tid
 * exists it is considered an error. Terminating the main thread
 * (tid == 0) will result in the termination of the entire process using
 * exit(0) [after releasing the assigned library memory].
 * Return value: The function returns 0 if the thread was successfully
 * terminated and -1 otherwise. If a thread terminates itself or the main
 * thread is terminated, the function does not return.
*/
int uthread_terminate(int tid)
{
    int currRunning = scheduler.getRunning();
    int nextToRun;

    if(tid != 0)
    {
        if (manager.killThread(tid) != -1)                        //If thread exists.
        {
            nextToRun = scheduler.whosNextTermination(tid);
            if(nextToRun != currRunning){                          // If we should do a context switch.
                // manager.switch(nextToRun)
                //TODO: Why should this function not return if the thread terminates itself?
            }
            return 0;
        }
        std::cerr <<  LIB_ERROR_SYNTAX << "Thread doesn't exit." << std::endl;
        return -1;
    }
    //TODO: release assigned memory if exists.
    exit(0);
}


/*
 * Description: This function blocks the thread with ID tid. The thread may
 * be resumed later using uthread_resume. If no thread with ID tid exists it
 * is considered as an error. In addition, it is an error to try blocking the
 * main thread (tid == 0). If a thread blocks itself, a scheduling decision
 * should be made. Blocking a thread in BLOCKED state has no
 * effect and is not considered an error.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_block(int tid)
{
    int currRunning = scheduler.getRunning();
    int nextToRun;

    if (tid != 0)
    {
        if(manager.blockThread(tid) != -1){                 // If thread exists.
            nextToRun = scheduler.whosNextBlock(tid);
            if(nextToRun != currRunning){                   // If we should do a context switch.
                //manager.switch(nextToRun);
            }
        }
        std::cerr <<  LIB_ERROR_SYNTAX << "Thread doesn't exit." << std::endl;
    }
    std::cerr <<  LIB_ERROR_SYNTAX << "Blocking the main thread is forbidden." << std::endl;
    return -1;
}


/*
 * Description: This function resumes a blocked thread with ID tid and moves
 * it to the READY state if it's not synced. Resuming a thread in a RUNNING or READY state
 * has no effect and is not considered as an error. If no thread with
 * ID tid exists it is considered an error.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_resume(int tid)
{
    if (manager.unBlockThread(tid) != -1)
    {
        if((tid != scheduler.getRunning()) && !(scheduler.inReady(tid))){
            scheduler.appendTid(tid);
        }
        return 0;
    }
    std::cerr <<  LIB_ERROR_SYNTAX << "Thread doesn't exit." << std::endl;
    return -1;
}


/*
 * Description: This function blocks the RUNNING thread for user specified micro-seconds (virtual time).
 * It is considered an error if the main thread (tid==0) calls this function.
 * Immediately after the RUNNING thread transitions to the BLOCKED state a scheduling decision
 * should be made.
 * Return value: On success, return 0. On failure, return -1.
*/
int uthread_sleep(int usec)
{
    int currTid = scheduler.getRunning();
    if (currTid != 0)
    {
        manager.putThreadToSleep(currTid);
        //TODO: scheduling decision.
        return 0;
    }
    //TODO: print Error- putting main thread to sleep.
    return -1;
}

/*
 * Description: This function returns the thread ID of the calling thread.
 * Return value: The ID of the calling thread.
*/
int uthread_get_tid()
{
    return scheduler.getRunning();
}


/*
 * Description: This function returns the total number of quantums since
 * the library was initialized, including the current quantum.
 * Right after the call to uthread_init, the value should be 1.
 * Each time a new quantum starts, regardless of the reason, this number
 * should be increased by 1.
 * Return value: The total number of quantums.
*/
int uthread_get_total_quantums()
{
    return totalQuants;
}


/*
 * Description: This function returns the number of quantums the thread with
 * ID tid was in RUNNING state. On the first time a thread runs, the function
 * should return 1. Every additional quantum that the thread starts should
 * increase this value by 1 (so if the thread with ID tid is in RUNNING state
 * when this function is called, include also the current quantum). If no
 * thread with ID tid exists it is considered an error.
 * Return value: On success, return the number of quantums of the thread with ID tid.
 * 			     On failure, return -1.
*/
int uthread_get_quantums(int tid)
{
    int threadQuants = manager.getThreadQuants(tid);
    if (threadQuants != -1)  // If thread exists.
    {
        return threadQuants;
    }
    std::cerr <<  LIB_ERROR_SYNTAX << "Thread doesn't exit." << std::endl;
    return -1;
}