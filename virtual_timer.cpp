//
// Created by Jonathan Weiss on 2019-04-05.
//

#include "virtual_timer.h"

virtual_timer::virtual_timer(int quantum): _quantum(quantum), _timer(){}

int virtual_timer::start() {
    int secondsPart = _quantum / CONVERTION_CONST_MSEC_SEC;   // down round.
    int microSecondsPart = _quantum - (secondsPart * CONVERTION_CONST_MSEC_SEC);

    // Sets the _timer to run a quantum microseconds:
    _timer.it_value.tv_sec = secondsPart;
    _timer.it_value.tv_usec = microSecondsPart;

    if (setitimer (ITIMER_VIRTUAL, &_timer, nullptr)) {
        return -1;
    }
    return 0;
}

int virtual_timer::zero() {
    _timer.it_value.tv_sec = 0;
    _timer.it_value.tv_usec = 0;
    if (setitimer (ITIMER_VIRTUAL, &_timer, nullptr)) {
        return -1;
    }
}
