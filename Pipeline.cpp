#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class Pipeline {
public:
    Pipeline() : stop(false) {
        worker = std::thread([this] { this->process(); });
    }

    ~Pipeline() {
        {
            std::lock_guard<std::mutex> lock(mutex);
            stop = true;
        }
        condVar.notify_all();
        worker.join();
    }

    void enqueueTask(std::function<void()> task) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            tasks.push(task);
        }
        condVar.notify_one();
    }

private:
    void process() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(mutex);
                condVar.wait(lock, [this] { return stop || !tasks.empty(); });
                if (stop && tasks.empty()) return;
                task = tasks.front();
                tasks.pop();
            }
            task();
        }
    }

    std::queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::condition_variable condVar;
    std::thread worker;
    bool stop;
};
