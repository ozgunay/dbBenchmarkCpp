// Timer.h

#ifndef _DBBENCHMARK_TIMER_H_
#define _DBBENCHMARK_TIMER_H_

#include <chrono>

namespace dbbenchmark{
namespace utility {

template <typename T>
class Timer {
public:
  void Start() {
    time_ = Clock::now();
  }

  T End() {
    Duration span;
    Clock::time_point t = Clock::now();
    span = std::chrono::duration_cast<Duration>(t - time_);
    return span.count();
  }

private:
  typedef std::chrono::high_resolution_clock Clock;
  typedef std::chrono::duration<T> Duration;

  Clock::time_point time_;
};

} // namespace utility
}  // namespace dbtester

#endif // _DBBENCHMARK_TIMER_H_

