#ifndef TEMPEX2_SCHEDULER_H
#define TEMPEX2_SCHEDULER_H

#include <list>

class scheduler
{
    std::list<int> _ready;
    int _running;

public:

    /**
     * Initializes a new scheduler object with an empty ready queue and _running=0.
     */
    scheduler();

    /**
     * Decides who will be the next thread to run in the case the thread with tid got blocked. This method also
     * updates the scheduler's internal state according to the decision, and sets _running and ready accordingly.
     * @return the next tid to run.
     */
    int whosNextBlock(int tid);

    /**
     * Decides who will be the next thread to run in the case of a timeout. This method also
     * updates the scheduler's internal state according to the decision, and sets _running and ready accordingly.
     * @return the next tid to run.
     */
    int whosNextTimeout();

    /**
     * Decides who will be the next thread to run in the case the thread with this tid has terminated. This method also
     * updates the scheduler's internal state according to the decision, and sets _running and ready accordingly.
     * @return the next tid to run.
     */
    int whosNextTermination(int tid);

    /**
     * Decides who will be the next thread to run in the case the running should sleep. This method also
     * updates the scheduler's internal state according to the decision, and sets _running and ready accordingly.
     * @return the next tid to run.
     */
    int whosNextSleep();

    /**
     * Returns true if the thread with this tid is in _ready.
     * @param tid
     * @return
     */
    bool inReady(int tid);

    void appendTid(int tid);

    /**
     * Returns which thread in is the CPU right now.
     * @return The id of the running thread.
     */
    int getRunning();

};


#endif //TEMPEX2_SCHEDULER_H
