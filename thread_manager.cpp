
#include <iostream>
#include "thread.h"
#include "thread_manager.h"

typedef unsigned long address_t;

//-----Private helpers-----------------------------------------------------------------------------

thread *thread_manager::findThread(const int tid)
{
    auto pairTidThread = _threads.find(tid);
    if (pairTidThread != _threads.end())
    {
        return pairTidThread->second;
    }
    return nullptr;
}

int thread_manager::getSmallestTid()
{
    if (!_usedIds.empty())
    {
        int smallestId = _usedIds.top();
        _usedIds.pop();
        return smallestId;
    }
    return ++_largestId;
}


//--- Constructors---------------------------------------------------------------------------------

thread_manager::thread_manager(const int quantum_usecs, const int maxThreadNum,
                               const int stackSize): _maxThreadNum(maxThreadNum - 1),_stackSize(stackSize),
                                                    _quantumUsecs(quantum_usecs), _largestId(0),
                                                    _usedIds(), _threads()
{

    //create representation of main thread:
    auto *mainThread = new thread();
    mainThread->updateQuants();
    _threads.insert({0, mainThread});
}

//----Class functionality--------------------------------------------------------------------------

int thread_manager::createThread(void (*f)())
{
    if (_threads.size() < _maxThreadNum)
    {
        int newTid = getSmallestTid();
        auto *newThread = new thread();
        newThread->setupThread(f, _stackSize);
        _threads.insert({newTid, newThread});
        return newTid;
    }
    return -1;
}

int thread_manager::killThread(const int tid)
{
    thread *threadWithTid = findThread(tid);
    if (threadWithTid != nullptr)
    {
        delete threadWithTid;
        _threads.erase(tid);
        _usedIds.push(tid); //recycles this tid
        return 0;
    }
    return -1;
}

int thread_manager::blockThread(const int tid)
{
    thread *threadWithTid = findThread(tid);
    if (threadWithTid != nullptr)
    {
        threadWithTid->setBlocked(true);
        return 0;
    }
    return -1;
}

int thread_manager::unBlockThread(const int tid)
{
    thread *threadWithTid = findThread(tid);
    if (threadWithTid != nullptr)
    {
        threadWithTid->setBlocked(false);
        return 0;
    }
    return -1;
}

bool thread_manager::isThreadBlocked(const int tid) //TODO: check with sleep impl
{
    thread *threadWithTid = findThread(tid);
    if (threadWithTid != nullptr)
    {
        return threadWithTid->getBlocked();
    }
    return false;
}

void thread_manager::putThreadToSleep(const int tid)
{
    //no need to check for existence-
    // made only on the running (and therefore existing) thread.
    thread *threadWithTid = findThread(tid);
    threadWithTid->setSleep(true);
}

int thread_manager::wakeThread(const int tid)
{
    thread *threadWithTid = findThread(tid);
    if (threadWithTid != nullptr)
    {
        threadWithTid->setSleep(false);
        return 0;
    }
    return -1;
}

bool thread_manager::isThreadAsleep(const int tid) //TODO: check with sleep impl
{
    thread *threadWithTid = findThread(tid);
    if (threadWithTid != nullptr)
    {
        return threadWithTid->getSleep();
    }
    return false;
}

int thread_manager::getThreadQuants(const int tid){
    thread *threadWithTid = findThread(tid);
    if (threadWithTid != nullptr)
    {
        return threadWithTid->getQuants();
    }
    return -1;
}

void thread_manager::switchContext(int currTid, int nextTid)
{
    thread *currThread = thread_manager::findThread(currTid);
    thread *nextThread = thread_manager::findThread(nextTid);
    assert(currThread != nullptr && nextThread != nullptr);

    _threads[nextTid]->updateQuants();

    if (currTid != nextTid)
    {
        int retVal = sigsetjmp(currThread->_env, 1);
        if (retVal == 0)
        {
            std::cerr << "- switch from " << currTid << " to " << nextTid << " -" << std::endl;
            siglongjmp(nextThread->_env, 1);
        }
    }


}