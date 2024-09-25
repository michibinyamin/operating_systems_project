#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "MSTAlgorithm.hpp"
class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();

    void enqueue(Graph g, int client_fd);

private:
    std::vector<std::thread> workers;
    std::queue<std::pair<Graph, int>> tasks;
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

void ThreadPool::enqueue(Graph g, int client_fd) {
    {
        std::lock_guard<std::mutex> lock(mtx);
        tasks.emplace(g,client_fd);
    }
    cv.notify_one();
}

void ThreadPool::workerFunction() {
    while (!stop) {
        std::pair<Graph, int> task;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this] { return !tasks.empty() || stop; });

            if (stop) return;

            task = tasks.front();
            tasks.pop();
        }

        // Process the task
        Total_weight(task.first, task.second);
        Longest_distance(task.first, task.second);
        Average_distance(task.first, task.second);
        Shortest_distance(task.first, task.second);
    }
}

class LeaderFollower {
public:
    LeaderFollower(int numThreads) : threadPool(numThreads) {}

    void addTask(Graph g, int client_fd) {
        threadPool.enqueue(g,client_fd);
    }

private:
    ThreadPool threadPool;
};
