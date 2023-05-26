#ifndef BARRIER_H
#define BARRIER_H

#include <mutex>
#include <atomic>
#include <memory>
#include <condition_variable>

class Barrier
{
public:
    explicit Barrier(std::size_t n);

    Barrier(const Barrier&) noexcept = default;
    Barrier& operator=(const Barrier&) noexcept = default;
    Barrier(Barrier&&) noexcept = default;
    Barrier& operator=(Barrier&&) noexcept = default;

    void arrive() noexcept;
    void wait() noexcept;
    void arrive_and_wait() noexcept;
    void reset(std::size_t n) noexcept;

private:
    struct inner {
        inner(std::size_t n) noexcept : waiting(n) {}

        std::mutex mtx;
        std::condition_variable cv;
        std::atomic<std::size_t> waiting;
    };

    std::shared_ptr<inner> _inner;
};

#endif // BARRIER_H
