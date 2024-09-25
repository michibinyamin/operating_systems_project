#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    void enqueue(int task);

private:
    std::vector<std::thread> workers;
    std::queue<int> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<bool> stop;

    void workerFunction();
};

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back(&ThreadPool::workerFunction, this);
    }
}

ThreadPool::~ThreadPool() {
    stop = true;
    cv.notify_all();
    for (auto& worker : workers) {
        worker.join();
    }
}

void ThreadPool::enqueue(int task) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        tasks.push(task);
    }
    cv.notify_one();
}

void ThreadPool::workerFunction() {
    while (!stop) {
        int task;

        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return !tasks.empty() || stop; });

            if (stop) return;

            task = tasks.front();
            tasks.pop();
        }

        // Process the task
        std::cout << "Processing task: " << task << std::endl;
    }
}

class LeaderFollower {
public:
    LeaderFollower(int numThreads) : threadPool(numThreads) {}

    void addTask(int task) {
        std::cout << "Leader adds task: " << task << std::endl;
        threadPool.enqueue(task);
    }

private:
    ThreadPool threadPool;
};

int main() {
    LeaderFollower lf(3); // Create Leader with a ThreadPool of 3 threads

    // Adding tasks
    for (int i = 1; i <= 5; ++i) {
        lf.addTask(i);
    }

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Give some time for processing

    return 0;
}
