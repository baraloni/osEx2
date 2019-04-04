#include "thread.h"

//thread::thread(void(*f)(void), int stackSize, int tid): _tid(tid), _isBlocked(false),
//                                                        _isSleeping(false), _quants(0),
//                                                        _pc((address_t)f){
//    char stack[stackSize];
//    _sp = (address_t)stack + stackSize - sizeof(address_t);
//}

void thread::setBlocked(bool isBlocked){
    _isBlocked = isBlocked;
}

bool thread::getBlocked(){
    return _isBlocked;
}

void thread::setSleep(bool isSleeping){
    _isSleeping = isSleeping;
}

bool thread::getSleep(){
    return _isSleeping;
}

int thread::getQuants(){
    return _quants;
}

void thread::updateQuants(){
    _quants++;
}
