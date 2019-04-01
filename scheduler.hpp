//
// Created by hp on 31/03/2019.
//
#include <deque>
#ifndef OSEX2_SCHEDULER_HPP
#define OSEX2_SCHEDULER_HPP


class scheduler
{
    std::deque<unsigned int> _ready;
    unsigned int _running;

public:

    /**
     * appends this tid to the _ready queue.
     */
    void appendTid(int tid){}

    /**
     * pops the top tid from the queue, and sets it as the _running tid.
     * @return -1 if failed, 0 otherwise.
     */
    int setNextRunning(){}

    int removeFromReady(){}

    int getRunning(){}

};


#endif //OSEX2_SCHEDULER_HPP
