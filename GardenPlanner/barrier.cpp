#include "barrier.h"

Barrier::Barrier(std::size_t n) :
    _inner(std::make_shared<inner>(n))
{
}

void Barrier::arrive() noexcept {
    _inner->waiting.fetch_sub(1, std::memory_order_release);
    _inner->cv.notify_all();
}

void Barrier::wait() noexcept {
    std::unique_lock lk(_inner->mtx);
    _inner->cv.wait(lk, [this] { return _inner->waiting.load(std::memory_order_acquire) == 0; });
}

void Barrier::arrive_and_wait() noexcept {
    arrive();
    wait();
}

void Barrier::reset(std::size_t n) noexcept {
    _inner->waiting.store(n, std::memory_order_release);
}
