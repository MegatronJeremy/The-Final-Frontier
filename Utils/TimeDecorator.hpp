#pragma once

#include <functional>
#include <chrono>
#include <iostream>

template<class>
struct ExeTime;

// Execution time decorator
template<class R, class... Args>
struct ExeTime<R(Args ...)> {
public:
    explicit ExeTime(std::function<R(Args...)> func) : f_(func) {}

    double operator()(Args ... args) {
        auto start = std::chrono::system_clock::now();
        f_(args...);
        auto end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        std::cout << elapsed.count() << " ns" << std::endl;

        return static_cast<double>(elapsed.count());
    }

private:
    std::function<R(Args ...)> f_;
};

template<class R, class... Args>
ExeTime<R(Args ...)> make_time_decorator(R (*f)(Args ...)) {
    return ExeTime<R(Args...)>(std::function<R(Args...)>(f));
}

template<class R, class... Args>
ExeTime<R(Args ...)> make_time_decorator(std::function<R(Args ...)> f) {
    return ExeTime<R(Args...)>(std::function<R(Args...)>(f));
}
