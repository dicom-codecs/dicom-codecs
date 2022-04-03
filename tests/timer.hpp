#include <chrono>

class Timer {
    public:
    Timer()
    {}

    void start() {
        start_ = std::chrono::high_resolution_clock::now();
    }

    double end() {
        end_ = std::chrono::high_resolution_clock::now();
        auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_);
        std::chrono::duration<double, std::milli> ms_double = end_ - start_;
        return ms_double.count();
    }
    
private:
    std::chrono::steady_clock::time_point start_;
    std::chrono::steady_clock::time_point end_;
};