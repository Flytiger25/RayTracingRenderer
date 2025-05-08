#include "thread/thread_pool.hpp"
#include <mutex>
#include <thread>

void ThreadPool::WorkerThread(ThreadPool *master) {
    while (master->alive == 1) {
        Task *task = master->getTask();
        if (task != nullptr) {
            task->run();
            master->pending_task_count--;
        } else {
            std::this_thread::yield();
        }
    }
}

ThreadPool::ThreadPool(int thread_count) {
    alive = 1;
    pending_task_count = 0;
    if (thread_count == 0) {
        thread_count = std::thread::hardware_concurrency(); // 赋值为cpu的线程数
    }

    for (int i = 0; i < thread_count; i++) {
        threads.push_back(std::thread(ThreadPool::WorkerThread, this));
    }
}

ThreadPool::~ThreadPool() {
    wait();
    alive = 0;
    for (auto &thread : threads) {
        thread.join();
    }
    threads.clear();
}

class ParallelForWork : public Task {
public:
    ParallelForWork(int x, int y, const std::function<void(int, int)> &lambda)
        : x(x), y(y), lambda(lambda) {}

    void run() override {
        lambda(x, y);
    }

private:
    int x, y;
    std::function<void(int, int)> lambda;
};


void ThreadPool::parallel_for(int width, int height, const std::function<void(int, int)> &lambda) {
    //std::lock_guard<std::mutex> guard(lock);
    Guard guard(spin_lock);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            pending_task_count++;
            tasks.push(new ParallelForWork(x, y, lambda));
        }
    }
}

void ThreadPool::wait() const {
    while (pending_task_count > 0) {
        std::this_thread::yield();
    }
}

void ThreadPool::addTask(Task *task) {
    //std::lock_guard<std::mutex> guard(lock);
    Guard guard(spin_lock);

    pending_task_count++;
    tasks.push(task);
}

Task* ThreadPool::getTask() {
    //std::lock_guard<std::mutex> guard(lock);
    Guard guard(spin_lock);

    if (tasks.empty()) {
        return nullptr;
    }
    Task *task = tasks.front();
    tasks.pop();
    return task;
}