#include "scheduler.h"

void scheduler::appendTid(int tid) {
    _ready.push_back(tid);
}

int scheduler::removeFromReady(int tid) {
    _ready.remove(tid);
}

int scheduler::getRunning() {
    return _running;
}

int scheduler::whoRunsNext() {
    return _ready.front();
}

void scheduler::switchThreads() {
    _running = whoRunsNext();
    _ready.pop_front();
}


