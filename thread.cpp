#include "thread.h"
#define JB_SP 6
#define JB_PC 7

thread::thread(sigjmp_buf& env)
        : _threadEnv(env), _quants(0), _isBlocked(false), _isSleeping(false) {}

void thread::setupThread(void(*f)(), int stackSize)
{
    char stack[stackSize];
    address_t sp = (address_t)stack + stackSize - sizeof(address_t);
    auto pc = (address_t)f;
    sigsetjmp(_threadEnv, 1);
    (_threadEnv->__jmpbuf)[JB_SP] = translateAddress(sp);
    (_threadEnv->__jmpbuf)[JB_PC] = translateAddress(pc);
    sigemptyset(&_threadEnv->__saved_mask); //TODO: no signals to check, success:0, fail: -1
}

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

void thread::updateQuants(){
    _quants++;
}

int thread::getQuants(){
    return _quants;
}

address_t thread::translateAddress(address_t addr)
{
    address_t ret;
    asm volatile("xor    %%fs:0x30,%0\n"
                 "rol    $0x11,%0\n"
                 : "=g" (ret)
                 : "0" (addr));
    return ret;
}