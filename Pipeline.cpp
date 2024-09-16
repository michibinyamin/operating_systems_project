#include <iostream>
#include <vector>
#include <functional>

// Define a pipeline stage
template<typename T>
using PipelineStage = std::function<T(T)>;

// Pipeline class
template<typename T>
class Pipeline {
public:
    void addStage(PipelineStage<T> stage) {
        stages.push_back(stage);
    }

    T process(T input) {
        T result = input;
        for (auto& stage : stages) {
            result = stage(result);
        }
        return result;
    }

private:
    std::vector<PipelineStage<T>> stages;
};

// Example usage
int main() {
    Pipeline<int> pipeline;

    // Add stages to the pipeline
    pipeline.addStage([](int x) { return x + 1; });
    pipeline.addStage([](int x) { return x * 2; });
    pipeline.addStage([](int x) { return x - 3; });

    // Process input through the pipeline
    int input = 5;
    int result = pipeline.process(input);

    std::cout << "Result: " << result << std::endl; // Should print 9
    return 0;
}
