#include "thread/thread_pool.hpp"
#include <mutex>
#include <thread>

ThreadPool thread_pool {};

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
    ParallelForWork(int x, int y, int chunk_width, int chunk_height, const std::function<void(int, int)> &lambda)
        : x(x), y(y), chunk_width(chunk_width), chunk_height(chunk_height), lambda(lambda) {}

    void run() override {
        for (int i = 0; i < chunk_width; i++) {
            for (int j = 0; j < chunk_height; j++) {
                lambda(x + i, y + j);
            }
        }
    }

private:
    int x, y;
    int chunk_width, chunk_height;
    std::function<void(int, int)> lambda;
};


void ThreadPool::parallel_for(int width, int height, const std::function<void(int, int)> &lambda) {
    //std::lock_guard<std::mutex> guard(lock);
    Guard guard(spin_lock);

    // 分块添加任务
    // 计算每个线程的块大小
    float chunk_width_float = static_cast<float>(width) / sqrt(16) / sqrt(threads.size());
    float chunk_height_float = static_cast<float>(height) / sqrt(16) / sqrt(threads.size());
    int chunk_width = std::ceil(chunk_width_float);
    int chunk_height = std::ceil(chunk_height_float);
    
    for (int x = 0; x < width; x+=chunk_width) {
        for (int y = 0; y < height; y+=chunk_height) {
            pending_task_count++;
            // 如果块的宽度超过了图像的宽度，则将块的宽度设置为图像的宽度
            if (x + chunk_width > width) {
                chunk_width = width - x;
            }
            // 如果块的高度超过了图像的高度，则将块的高度设置为图像的高度
            if (y + chunk_height > height) {
                chunk_height = height - y;
            }
            tasks.push(new ParallelForWork(x, y, chunk_width, chunk_height, lambda));
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