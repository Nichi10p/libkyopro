/* Timer */
#include <chrono>
#include <iostream>

class timeit {
private:
    static inline int used_timer_id = 0;
    const int this_timer_id;
    const std::chrono::system_clock::time_point begin;
public:
    timeit()
    : this_timer_id(used_timer_id + 1), begin(std::chrono::system_clock::now()) {
        std::clog << "Timer(" << this_timer_id << ") start." << std::endl;
        used_timer_id++;
    }
    ~timeit() {
        const auto end = std::chrono::system_clock::now();
        const auto dtime = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
        std::clog << "Timer(" << this_timer_id << ") stop. ";
        std::clog << "duration: " << dtime.count() / 1'000'000 << '.' << dtime.count() % 1'000'000 << " sec." << std::endl;
    }
};
#define timeit_begin(name) auto name = new timeit()
#define timeit_end(name) delete name
