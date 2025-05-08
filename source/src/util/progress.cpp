#include "util/progress.hpp"
#include <iostream>

Progress::Progress(int total, int step) : total(total), current(0), percent(0), last_percent(0), step(step) {
    std::cout << "0%" << std::endl;
};

void Progress::update(int count) {
    Guard guard(spin_lock);

    current += count;
    percent = 100 * static_cast<float>(current) / static_cast<float>(total);
    if ((percent - last_percent == step) || (percent == 100)) {
        last_percent = percent;
        std::cout << percent << "%" << std::endl;
    }

}