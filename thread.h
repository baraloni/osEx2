#ifndef TEMPEX2_THREAD_H
#define TEMPEX2_THREAD_H

/**
 * This class is a "ticket" which saves on it the thread's information. It is supposed to be
 * somehow similar to a PCB entry, but for threads.
 */
class thread
{
    int _tid;
    typedef unsigned int address_t; // holds the stack pointer and the program counter
    address_t _sp, _pc;
    int _quants; // holds the number of quantums this thread spent as RUNNING.
    bool _isBlocked;
    bool _isSleeping;


public:
    /**
     * Creates a new thread object.
     * @param f : The function the thread should execute.
     * @param stackSize: The size of the thread's size.
     * @param tid
     */
    thread(void(*f)(void), int stackSize, int tid);

    /**
     * Updates the _setBlocked parameter.
     * @param isBlocked
     */
    void setBlocked(bool isBlocked);

    /**
     * Returns True iff the thread is blocked.
     * @return
     */
    bool getBlocked();

    /**
     * Updates the _setSleep parameter.
     * @param isSleeping
     */
    void setSleep(bool isSleeping);

    /**
     * Return True iff the thread is sleeping.
     * @return
     */
    bool getSleep();

    /**
     * Returns the number of quantums in which the thread had been active.
     * @return
     */
    int getQuants();

    /**
     * Increments the value of _quants in 1.
     */
    void updateQuants();
};


#endif //TEMPEX2_THREAD_H
