#include "uthreads.hpp"
#include "thread_manager.hpp"
#include "scheduler.hpp"

#include <queue>
#include <unordered_map>

//-------------Error Massages:
#define LIB_ERROR_SYNTAX "thread library error: "
#define SYS_ERROR_SYNTAX "system error: "


//-------------Static Globals:
static thread_manager *manager; // TODO: by pointer or value?
static scheduler *scheduler; // TODO: by pointer or value?
static int totalQuants = 0;

//-------------Timer: //TODO: TIMER DECLARATION AND SETTING & HANDLER & totalQuants maintenance




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
        manager = new thread_manager(quantum_usecs, MAX_THREAD_NUM, STACK_SIZE);
        scheduler = new scheduler();
        // initialize library:
        int mainThreadId = manager->createThread(); // TODO: what is f() for the main thread?
        if (mainThreadId != -1)
        {
            scheduler->appendTid(mainThreadId);
            scheduler->setNextRunning();
            return 0;
        }
        //TODO: print Error-
        //TODO: causes the number of concurrent threads to exceeds the limit of _maxNumOfThreads
        return -1;
    }
    // TODO: Print Error- quantum_usecs should be a non-negative integer.
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
    int newTid = manager->createThread(f);
    if(newTid != -1){
        scheduler->appendTid(newTid);
        return newTid;
    }
    // TODO: Print Error-
    //TODO: causes the number of concurrent threads to exceeds the limit of _maxNumOfThreads

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
    if(tid != 0)
    {
        if (manager->killThread(tid) != -1)
        {
            if (scheduler->getRunning() == tid)
            {
                //TODO: static func that switches in the manners of : schedule, manager.
            }
            else
            {
                scheduler->removeFromReady();
                return 0;
            }
        }
        // TODO: Print Error- no such thread exists.
        return -1;
    }
    //TODO: release assigned memory. is there memory to be freed?
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
    if (tid != 0)
    {
        if (scheduler->getRunning() == tid)
        {
            //TODO: scheduling decision.
        }
        else
        {
            if (manager->blockThread(tid) != -1)
            {
                scheduler->removeFromReady();
                return 0;
            }
            //TODO: print Error- no such thread exists.
            return -1;
        }
    }
    //TODO: print Error- blocking main thread.
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
    if (manager->unBlockThread(tid) != -1)
    {
        scheduler->appendTid(tid);
        return 0;
    }
    //TODO: print Error- no such thread exists.
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
    int currTid = scheduler->getRunning();
    if (currTid != 0)
    {
        manager->putThreadToSleep(currTid);
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
    return scheduler->getRunning();
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
    int threadQuants = manager->getThreadQuants(tid);
    if (threadQuants != -1)
    {
        return threadQuants;
    }
    //TODO: print Error- no such thread exists.
    return -1;
}
