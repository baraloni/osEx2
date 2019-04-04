#include "scheduler.h"

scheduler::scheduler(){
    _running = 0;
}

void scheduler::appendTid(int tid) {
    _ready.push_back(tid);
}

int scheduler::getRunning() {
    return _running;
}



