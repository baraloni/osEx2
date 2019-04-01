//
// Created by hp on 31/03/2019.
//

#ifndef OSEX2_THREAD_HPP
#define OSEX2_THREAD_HPP

class thread
{
    int _tid;

    typedef unsigned int address_t;
    /** holds the stack pointer and the program counter*/
    address_t _sp, _pc; //TODO private/public

    /** holds the number of quantums this thread spent as RUNNING*/
    int _quants; //TODO: check type

    bool _isBlocked;

    bool _isSleeping;


public:

    thread()
    {
        // TODO ask about setting the _sp - how to choose a base address
    };

    int setBlocked(bool isBlocked){}
    bool getBlocked(){}

    int setSleep(bool isSleeping){}
    bool getSleep(){}

    int getQuants(){}
};


#endif //OSEX2_THREAD_HPP
