// TASKS:
// TODO: SLEEP! (NOY)
// TODO: Protect critical code (NEXT)
// TODO: Implement Switch in manager.(BAR)
// TODO: Take care of system errors. (BAR)
// TODO: Test memory allocations. (NOY(Uthreads&scheduler) BAR (manager&thread))
// TODO: How do we return from a switch? Does it has to be the last operation in the calling function? (BAR)



#include "uthreads.h"
#include "thread_manager.h"
#include "scheduler.h"
#include "virtual_timer.h"

#include <queue>
#include <unordered_map>
#include <iostream>
#include <signal.h>


//-------------Error Massages:
#define LIB_ERROR_SYNTAX "thread library error: "
#define SYS_ERROR_SYNTAX "system error: "


//-------------Static Globals:
static struct sigaction sa;
static thread_manager* manager;
static scheduler* scheduler;
static virtual_timer* vTimer;
static int totalQuants = 0;
static sigset_t toBlock;        // A set of signals to block where critical code is executed.

//------------Memory Management
/**
 * Clears the library resources.
 */
static void clearMem(){
    delete scheduler;
    delete manager;
    delete vTimer;
}


/**
 * Clears the system memory and exits.
 * @param errMsg: The error message to be printed in the case of a system call.
 */
static void exitProg(const std::string& errMsg){
    clearMem();
    std::cerr << SYS_ERROR_SYNTAX << errMsg << std::endl;
    exit(1);
}

//-------------Masking:
static void maskSignals(){
    if(sigprocmask(SIG_SETMASK, &toBlock, nullptr) < 0){
        exitProg("Failed to set signal masking.");
    }
}

static void unmaskSignals(){
    if(sigprocmask(SIG_UNBLOCK, &toBlock, nullptr) < 0){
        exitProg("Failed to undo signal masking.");
    }
}

//-------------Signal Handlers:
// TODO: What should I do with sig?
// TODO: Should I add protection to this code? (use sigprogmask? add mask to sigaction?)
static void handleQuantumTimeout(int sig){
    // Zero the _timer:
    if (vTimer->zero() < 0){
        exitProg("Failed to zero _timer.");
    }

    // Do a context switch:
    int nextToRun = scheduler->whosNextTimeout();
    // manager.switch(nextToRun);

    // Start the _timer again & update totalQuants:
    if(vTimer->start() < 0){
        exitProg("Failed to start _timer.");
    }
    totalQuants++;
}

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
        // Create global functionality holders:
        manager = new thread_manager(quantum_usecs, MAX_THREAD_NUM, STACK_SIZE);
        vTimer = new virtual_timer(quantum_usecs);
        scheduler = new class scheduler;
        sa = {0};

        // Initialize the signal set to block:
        if(sigaddset(&toBlock, SIGVTALRM) < 0){
            exitProg("Failed to initialize signal set to block.");
        }

        // Set signal handlers:
        sa.sa_handler = &handleQuantumTimeout;
        sa.sa_flags = 0; // Verifies that no flags are applied.
        if(sigaction(SIGVTALRM, &sa, nullptr) < 0){
            exitProg("sigaction had failed.");
        }

        // Start _timer & update the quantum counting:
        if(vTimer->start() < 0){
            exitProg("Failed to start _timer.");
        }
        totalQuants++;
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
    int newTid = manager->createThread(f);

    if(newTid != -1){
        scheduler->addThread(newTid);
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
    int currRunning = scheduler->getRunning();
    int nextToRun;

    if(tid != 0)
    {
        if (manager->killThread(tid) != -1)                        //If thread exists.
        {
            nextToRun = scheduler->whosNextTermination(tid);
            if(nextToRun != currRunning){                          // If we should do a context switch.
                if (vTimer->zero() < 0){
                    exitProg("Failed to zero _timer.");
                }
                // manager.switch(nextToRun)
                if(vTimer->start() < 0){
                    exitProg("Failed to start _timer.");
                }
                totalQuants++;
            }
            return 0;
        }
        std::cerr <<  LIB_ERROR_SYNTAX << "Thread doesn't exit." << std::endl;
        return -1;
    }
    clearMem();
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
    int currRunning = scheduler->getRunning();
    int nextToRun;

    if (tid != 0)
    {
        if(manager->blockThread(tid) != -1){                 // If thread exists.
            nextToRun = scheduler->whosNextBlock(tid);
            if(nextToRun != currRunning){                   // If we should do a context switch.
                if (vTimer->zero() < 0){
                    exitProg("Failed to zero _timer.");
                }
                //manager.switch(nextToRun);
                if(vTimer->start() < 0){
                    exitProg("Failed to start _timer.");
                }
                totalQuants++;
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
    if (manager->unBlockThread(tid) != -1)
    {
       scheduler->addThread(tid);
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
    if (threadQuants != -1)  // If thread exists.
    {
        return threadQuants;
    }
    std::cerr <<  LIB_ERROR_SYNTAX << "Thread doesn't exit." << std::endl;
    return -1;
}