//
// Created by Jonathan Weiss on 2019-04-05.
//

#ifndef EX2_VIRTUAL_TIMER_H
#define EX2_VIRTUAL_TIMER_H

static const int CONVERTION_CONST_MSEC_SEC = 1000000;
#include <sys/time.h>

class virtual_timer {
    int _quantum;
    struct itimerval _timer;

public:
    /**
     * Creates a virtual timer.
     * @param quantum: The number of micro seconds the timer should count.
     */
    explicit virtual_timer(int quantum);

    /**
     * Starts the timer to run quantum microseconds.
     * @return -1 in case of a system error.
     */
    int start();

    /**
     * Zeros the timer.
     */
    void zero();
};


#endif //EX2_VIRTUAL_TIMER_H
