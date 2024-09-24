#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ActiveObject {
public:
    using Task = std::function<void(int)>;

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

    void send(int data) {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push(data);
        cv_.notify_one();
    }

private:
    void run() {
        while (true) {
            int data;
            {
                std::unique_lock<std::mutex> lock(mutex_);
                cv_.wait(lock, [this]() { return stop_ || !queue_.empty(); });
                if (stop_ && queue_.empty()) return;
                data = queue_.front();
                queue_.pop();
            }
            task_(data);
        }
    }

    Task task_;
    std::thread thread_;
    std::queue<int> queue_;
    std::mutex mutex_;
    std::condition_variable cv_;
    bool stop_;
};

class Pipeline {
public:
    void addStage(ActiveObject::Task task) {
        stages_.emplace_back(task);
    }

    void execute(int data) {
        for (size_t i = 0; i < stages_.size(); ++i) {
            if (i == 0) {
                stages_[i].send(data);
            } else {
                stages_[i - 1].send(data);
            }
        }
    }

private:
    std::vector<ActiveObject> stages_;
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
