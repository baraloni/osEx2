//TODO: Check system calls/ std library calls.
//TODO: delete inReady
// TODO: delete appendTid

#include <iostream>
#include "scheduler.h"

/*------------- CONSTRUCTORS ------------*/
scheduler::scheduler(){
    _running = 0;
}

/*------------- PRIVATE -------------*/
void scheduler::_replaceRunning() {
    _running = _ready.front();
    _ready.pop_front();
}

void scheduler::_handleBlockOrTermination(int tid) {
    if(tid == _running){
        _replaceRunning();
    }
    else{
        // If tid is in _ready, remove it from there:
        _ready.remove(tid);
    }
}

/*------------- PUBLIC -------------*/
int scheduler::getRunning() {
    return _running;
}

int scheduler::whosNextBlock(int tid) {
    _handleBlockOrTermination(tid);
    return _running;
}

int scheduler::whosNextTimeout() {
    _ready.push_back(_running);
    _replaceRunning();
    return _running;
}

int scheduler::whosNextTermination(int tid) {
    _handleBlockOrTermination(tid);
    return _running;
}

int scheduler::whosNextSleep() {
    _replaceRunning();
    return _running;
}


void scheduler::addThread(int tid) {
    // If tid is not already in _ready or _running, add it to the end of _ready:
    auto findIter = std::find(_ready.begin(), _ready.end(), tid);
    if((tid != _running) && (findIter==_ready.end())){
        _ready.push_back(tid);
    }
}

void scheduler::printReady() {
    for(std::__1::__list_iterator<int, void *>::value_type & iter : _ready){
        std::cout << iter << "; ";
    }
    std::cout << std::endl;
}


// TO REMOVE:
bool scheduler::inReady(int tid) {
    return false;
}

void scheduler::appendTid(int tid) {
    _ready.push_back(tid);
}
