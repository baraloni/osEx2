#ifndef TEMPEX2_SCHEDULER_H
#define TEMPEX2_SCHEDULER_H

#include <list>

class scheduler
{
    std::list<int> _ready;
    int _running;

public:


    /**
     * appends this tid to the end of the _ready queue.
     */
    void appendTid(int tid);

    /**
     * Decides which thread is the next to run.
     * @return The relevant tid.
     */
    int whoRunsNext();

    /**
     * Updates the internal state of the scheduler in the case a switch occured.
     * @return
     */
    void switchThreads();

    /**
     * Removes the thread with this tid from ready.
     * @return
     */
    int removeFromReady(int tid);

    /**
     * Returns which thread in is the CPU right now.
     * @return The id of the running thread.
     */
    int getRunning();

};


#endif //TEMPEX2_SCHEDULER_H
