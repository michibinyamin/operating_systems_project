#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include "MSTAlgorithm.hpp"

//class Graph;

class ActiveObject {
public:
    using Task = std::function<void(Graph Tree, int client_fd)>;

    ActiveObject(Task task) : task_(task), stop_(false) {
        thread_ = std::thread(&ActiveObject::run, this);
    }

    ~ActiveObject() {
        {
            std::unique_lock<std::mutex> lock(mutex_);
            stop_ = true;
        }
        cv_.notify_all();
        thread_.join();
    }

    void send(Graph tree, int client_fd) {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.emplace(tree, client_fd);
        cv_.notify_one();
    }

private:
    void run() {
        while (true) {
            std::pair<Graph, int> data;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                cv_.wait(lock, [this]() { return stop_ || !queue_.empty(); });
                if (stop_ && queue_.empty()) return;
                data = queue_.front();
                queue_.pop();
            }
            task_(data.first, data.second);
        }
    }

    Task task_;
    std::thread thread_;
    std::queue<std::pair<Graph, int>> queue_;  // The function arguments
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stop_;
};

class Pipeline {
public:
    void addStage(ActiveObject::Task task) {
        stages_.emplace_back(std::make_shared<ActiveObject>(task));
    }

    void execute(Graph tree, int client_fd) {
        for (size_t i = 0; i < stages_.size(); ++i) {
            stages_[i]->send(tree, client_fd);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));   // To ensure they are sent in order
        }
    }

private:
    std::vector<std::shared_ptr<ActiveObject>> stages_;
};

// void stage1(int data) {
//     std::cout << "Stage 1: Processing " << data << "\n";
//     data += 1; // Modify data
//     std::this_thread::sleep_for(std::chrono::milliseconds(100));
// }

// void stage2(int data) {
//     std::cout << "Stage 2: Processing " << data << "\n";
//     data *= 2; // Modify data
//     std::this_thread::sleep_for(std::chrono::milliseconds(100));
// }

// int main() {
//     Pipeline pipeline;

//     ActiveObject stage1Obj(stage1);
//     ActiveObject stage2Obj(stage2);

//     pipeline.addStage(stage1Obj);
//     pipeline.addStage(stage2Obj);

//     pipeline.execute(5); // Start the pipeline with initial data

//     // Wait for a while to allow processing to complete
//     std::this_thread::sleep_for(std::chrono::seconds(1));
    
//     return 0;
// }
