#pragma once

#include "thread/spin_lock.hpp"

class Progress {
public:
    Progress(int total, int step = 1);
    void update(int count);

private:
    int total, current;
    int percent, last_percent, step;
    SpinLock spin_lock;
};