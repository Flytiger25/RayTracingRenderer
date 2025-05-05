#pragma once

#include <list>
#include <vector>
#include <thread>
#include <mutex>

class Task {
public:
    virtual void run() = 0;
};

class ThreadPool {
public:
    static void WorkerThread(ThreadPool *master);
    ThreadPool(int thread_count = 0);
    ~ThreadPool();

    void addTask(Task *task);
    Task* getTask();

private:
    bool alive;
    std::vector<std::thread> threads;
    std::list<Task *> tasks;
    std::mutex lock;
};