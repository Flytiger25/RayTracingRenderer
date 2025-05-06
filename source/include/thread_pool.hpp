#pragma once

#include <functional>
//#include <list>
#include <queue>
#include <vector>
#include <thread>
//#include <mutex>
#include "spin_lock.hpp"

class Task {
public:
    virtual void run() = 0;
};

class ThreadPool {
public:
    static void WorkerThread(ThreadPool *master);
    ThreadPool(int thread_count = 0);
    ~ThreadPool();

    void parallel_for(int width, int height, const std::function<void(int, int)> &lambda);
    void wait() const; // 让主线程主动等待所有任务完成

    void addTask(Task *task);
    Task* getTask();

private:
    std::atomic<int> alive;
    std::atomic<int> pending_task_count;
    std::vector<std::thread> threads;
    std::queue<Task *> tasks;
    //std::mutex lock;
    Spinlock spin_lock {};
};