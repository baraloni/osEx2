#ifndef OSEX2_HANDLER_HPP
#define OSEX2_HANDLER_HPP

#include <queue>
#include <unordered_map>
#include "thread.hpp"

class thread_manager
{
    int _maxThreadNum; //TODO static?
    int _stackSize;
    int _quantumUsecs;

    int _largestId;
    std::priority_queue<int, std::vector<int>, std::greater<int>> _usedIds;

    std::unordered_map<int, thread*> _threads;

    /**
     * checks if the supplied tid represents an existing thread.
     * @param tid representing a tid to search for
     * @return the address of the thread whose tid is the supplied one.
     * if no such thread exists, returns nullptr.
     */
    thread *exists(const int tid) const
    {
        auto threadAndTid = _threads.find(tid);
        if ( threadAndTid != _threads.end())
        {
            return &(_threads.find(tid)->second);
        }
        return nullptr;
    }

    /**
     * handles the tid assignments.
     * @return the smallest tid available for assignment.
     */
    int getSmallestTid()
    {
        if (!_usedIds.empty())
        {
            int smallestId = _usedIds.top();
            _usedIds.pop();
            return smallestId;
        }
        ++_largestId;
        return _largestId;
    }


public:

    /** constructs a thread_manager object*/
    thread_manager(const int quantum_usecs, const int maxThreadNum,
                   const int stackSize):_maxThreadNum(maxThreadNum),_stackSize(stackSize),
                   _quantumUsecs(quantum_usecs), _largestId(0), _usedIds(), _threads(){}

    typedef unsigned int address_t;

    /**
     *
     * @param f
     * @return
     */
    int createThread(void (*f)()) //TODO: help
    {
        if (_threads.size() < _maxThreadNum)
        {
            int newTid = getSmallestTid();
            thread newThread = thread();



            _threads.insert({getSmallestTid(), &newThread});
        }
        return -1;

    }

    /**
     * deletes the thread with the supplied tid, if exists.
     * @param tid
     * @return -1 if a thread of the supplied tid does not exist, 0 if it exists and erased successfully.
     */
    int killThread(const int tid)
    {
        thread *threadWithTid = exists(tid);
        if (threadWithTid != nullptr)
        {
            _threads.erase(tid); // TODO: should cause the element's destruction & stack?
            _usedIds.push(tid); //recycles this tid.
            return 0;
        }
        return -1;
    }

    int blockThread(const int tid)
    {
        thread *threadWithTid = exists(tid);
        if (threadWithTid != nullptr)
        {
            threadWithTid->setBlocked(true);
            return 0;
        }
        return -1;
    }

    int unBlockThread(const int tid)
    {
        thread *threadWithTid = exists(tid);
        if (threadWithTid != nullptr)
        {
            threadWithTid->setBlocked(false);
            return 0;
        }
        return -1;
    }

    bool isThreadBlocked(const int tid) //TODO: needed for scheduling (in case of of thread that is asleep and blocked)
    {
        thread *threadWithTid = exists(tid);
        if (threadWithTid != nullptr)
        {
            return threadWithTid->getBlocked();
        }
        return -1;
    }

    void putThreadToSleep(const int tid)

    {
        //no need to check for existence-
        // made only on the running (and therefore existing) thread.
        thread *threadWithTid = exists(tid);
        threadWithTid->setSleep(true);
    }

    int wakeThread(const int tid) //TODO: needed fot uthred_sleep when the timer ends
    {
        //no need to check for existence-
        // we make use of that when the timer of an existing thread ends.
        thread *threadWithTid = exists(tid);
        threadWithTid->setSleep(false);
    }

    bool isThreadAsleep(const int tid) //TODO: needed for scheduling (in case of of thread that is asleep and blocked)
    {
        thread *threadWithTid = exists(tid);
        if (threadWithTid != nullptr)
        {
            return threadWithTid->getSleep();
        }
        return -1;
    }


    int getThreadQuants(const int tid){
        thread *threadWithTid = exists(tid);
        if (threadWithTid != nullptr)
        {
            return threadWithTid->getQuants();
        }
        return -1;
    }

    int SwitchContext(){}
};


#endif //OSEX2_HANDLER_HPP
