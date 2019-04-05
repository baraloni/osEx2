//
// Created by Jonathan Weiss on 2019-04-05.
//

#ifndef EX2_VIRTUAL_TIMER_H
#define EX2_VIRTUAL_TIMER_H


class virtual_timer {
    int _quantLength;

public:
    explicit virtual_timer(int quantum);
    void startTimer();
    void zeroTimer();
};


#endif //EX2_VIRTUAL_TIMER_H
