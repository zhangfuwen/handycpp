//
// Created by zhangfuwen on 2022/3/23.
//

#ifndef HANDYCPP_TIME_H
#define HANDYCPP_TIME_H

#include <chrono>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>

namespace handycpp::time {

/**
 * @deprecated
 * @return
 */
inline double mono_clock_now() {
    struct timespec time1 = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &time1);
    return (double)time1.tv_sec + (double)time1.tv_nsec / 1000000000;
}
#define NANO 1'000'000'000L

inline bool operator <(const timespec& lhs, const timespec& rhs)
{
    if (lhs.tv_sec == rhs.tv_sec)
        return lhs.tv_nsec < rhs.tv_nsec;
    else
        return lhs.tv_sec < rhs.tv_sec;
}
inline bool operator ==(const timespec& lhs, const timespec& rhs)
{
    return lhs.tv_sec == rhs.tv_sec && lhs.tv_nsec == rhs.tv_nsec;
}
inline bool operator >(const timespec& lhs, const timespec& rhs)
{
    if (lhs.tv_sec == rhs.tv_sec)
        return lhs.tv_nsec > rhs.tv_nsec;
    else
        return lhs.tv_sec > rhs.tv_sec;
}

/**
 * lhs must be greater than or equal to rhs, otherwise all returned fields will be set zero.
 * @param lhs
 * @param rhs
 * @return
 */
inline timespec operator-(const timespec& lhs, const timespec& rhs) {
    timespec ret;
    if(lhs.tv_nsec > rhs.tv_nsec) {
        ret.tv_sec = lhs.tv_sec - rhs.tv_sec;
        ret.tv_nsec = lhs.tv_nsec - rhs.tv_nsec;
    } else {
        ret.tv_nsec = lhs.tv_nsec + 1'000'000'000 - rhs.tv_nsec;
        ret.tv_sec = lhs.tv_sec -1 - rhs.tv_nsec;
    }
    return ret;
}

inline timespec operator+(const timespec& lhs, const timespec& rhs) {
    timespec ret;
    if(1'000'000'000 - lhs.tv_nsec > rhs.tv_nsec) {
         ret.tv_sec = lhs.tv_sec + rhs.tv_sec + 1;
         ret.tv_nsec = lhs.tv_nsec + rhs.tv_nsec - 1'000'000'000;
    } else {
        ret.tv_sec = lhs.tv_sec + rhs.tv_sec;
        ret.tv_nsec = lhs.tv_nsec + rhs.tv_nsec;
    }
    return ret;
}




#if defined(__linux__)
namespace {

// buf needs to store 30 characters
inline int timespec2str(char *buf, uint len, struct timespec *ts) {
    int ret;
    struct tm t;

    tzset();
    if (localtime_r(&(ts->tv_sec), &t) == NULL)
        return 1;

    ret = strftime(buf, len, "%F %T", &t);
    if (ret == 0)
        return 2;
    len -= ret - 1;

    ret = snprintf(&buf[strlen(buf)], len, ".%09ld", ts->tv_nsec);
    if (ret >= (int)len)
        return 3;

    return 0;
}
} // namespace

/**
 * return string like [1234556 000456789], without brackets
 * @param t
 * @return
 */
inline std::string timespec_ns(const timespec & t, char sep = ' ') {
    char buf[50]; // enough for two long long and ten chars
    snprintf(buf, 50, "%lu%c%09lu", t.tv_sec, sep, t.tv_nsec);
    return buf;
}

/**
 * return string like [1234556 000456], without brackets
 * @param t
 * @return
 */
inline std::string timespec_us(const timespec & t, char sep = ' ') {
    char buf[50]; // enough for two long long and ten chars
    snprintf(buf, 50, "%lu%c%06lu", t.tv_sec, sep, t.tv_nsec/1000);
    return buf;
}

/**
 * return string like [1234556 023], without brackets
 * @param t
 * @return
 */
inline std::string timespec_ms(const timespec & t, char sep = ' ') {
    char buf[50]; // enough for two long long and ten chars
    snprintf(buf, 50, "%lu%c%03lu", t.tv_sec, sep, t.tv_nsec/1000'000);
    return buf;
}

#ifdef HANDYCPP_TEST
TEST_CASE("handycpp::time::timespec") {
    timespec t1, t2;
    clock_gettime(CLOCK_MONOTONIC, &t1);
    clock_gettime(CLOCK_MONOTONIC, &t2);
    CHECK(t1 == t1);
    CHECK(t2 > t1);
    CHECK(t1 < t2);
    timespec t3;
    t3.tv_sec = 1;
    t3.tv_nsec = 0;
    CHECK(t1 + t3 > t2);
    CHECK(t2 - t3 < t1);
    t3.tv_sec = 1;
    t3.tv_nsec = 123456789;
    using namespace std::string_literals;
    CHECK(timespec_ns(t3) == "1 123456789"s);
    CHECK(timespec_us(t3) == "1 123456"s);
    CHECK(timespec_ms(t3) == "1 123"s);
    t3.tv_nsec = 456789;
    CHECK(timespec_ns(t3) == "1 000456789"s);
    CHECK(timespec_us(t3) == "1 000456"s);
    CHECK(timespec_ms(t3) == "1 000"s);

}
#endif

/**
 * wall clock time to string
 * @return
 *    something like 2012-12-31 12:59:59.123456789;
 */
inline std::string wall_clock_now() {
    struct timespec time1 = {0, 0};
    clock_gettime(CLOCK_MONOTONIC, &time1);

    std::string ret = "2012-12-31 12:59:59.123456789";
    if (timespec2str(ret.data(), ret.size() + 1, &time1) != 0) {
        return "";
    } else {
        return ret;
    }
}

#endif // __linux__

/**
 * @usage
 *     Example:
 *
 * @code
 *      timer t;
 *      t.setTimeout([](){
 *          printf("timeout\n");
 *      }, 5000);
 *
 *      timer t2;
 *      t2.setInterval([]() {
 *          printf("triggered\n");
 *      }, 1000);
 *      sleep(10);
 *      t2.stop();
 *
 *
 *      in the above examples, t and t2 must not be destructed before timer expires or being stopped.
 */
class timer {
    volatile bool clear = false;

public:
    template <typename T> void setTimeout(T function, int delay_ms);
    template <typename T> void setInterval(T function, int inverval_ms);

    template <typename T, typename Rep, typename Period>
    void setTimeout(T function, std::chrono::duration<Rep, Period> delay);
    template <typename T, typename Rep, typename Period>
    void setInterval(T function, std::chrono::duration<Rep, Period> inverval);
    void stop();
    bool stopped();
};

template <typename T> void timer::setTimeout(T function, int delay_ms) {
    this->clear = false;
    std::thread t([=]() {
        if (this->clear)
            return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        if (this->clear)
            return;
        function();
        this->clear = true;
    });
    t.detach();
}

template <typename T> void timer::setInterval(T function, int inverval_ms) {
    this->clear = false;
    std::thread t([=]() {
        while (true) {
            if (this->clear)
                return;
            std::this_thread::sleep_for(std::chrono::milliseconds(inverval_ms));
            if (this->clear)
                return;
            function();
        }
    });
    t.detach();
}

template <typename T, typename Rep, typename Period>
void timer::setTimeout(T function, std::chrono::duration<Rep, Period> delay) {
    this->clear = false;
    std::thread t([=]() {
        if (this->clear)
            return;
        std::this_thread::sleep_for(delay);
        if (this->clear)
            return;
        function();
        this->clear = true;
    });
    t.detach();
}

template <typename T, typename Rep, typename Period>
void timer::setInterval(T function, std::chrono::duration<Rep, Period> inverval) {
    this->clear = false;
    std::thread t([=]() {
        while (true) {
            if (this->clear)
                return;
            std::this_thread::sleep_for(std::chrono::milliseconds(inverval));
            if (this->clear)
                return;
            function();
        }
    });
    t.detach();
}

inline void timer::stop() { this->clear = true; }
inline bool timer::stopped() { return this->clear = true; }

namespace cycle_clock {

/**
 * get current time using cpu's high precision always on timer.
 * @return return a number of timers ticks. to know how much time a time tick represents, use @link MeasureCyclesPerSecond
 */
inline int64_t Now() {
#if defined(BENCHMARK_OS_MACOSX)
    return mach_absolute_time();
#elif defined(__i386__)
    int64_t ret;
    __asm__ volatile("rdtsc" : "=A"(ret));
    return ret;
#elif defined(__x86_64__) || defined(__amd64__)
    uint64_t low, high;
    __asm__ volatile("rdtsc" : "=a"(low), "=d"(high));
    return (high << 32) | low;
#elif defined(__powerpc__) || defined(__ppc__)
    // This returns a time-base, which is not always precisely a cycle-count.
    int64_t tbl, tbu0, tbu1;
    asm("mftbu %0" : "=r"(tbu0));
    asm("mftb  %0" : "=r"(tbl));
    asm("mftbu %0" : "=r"(tbu1));
    tbl &= -static_cast<int64>(tbu0 == tbu1);
    // high 32 bits in tbu1; low 32 bits in tbl  (tbu0 is garbage)
    return (tbu1 << 32) | tbl;
#elif defined(__sparc__)
    int64_t tick;
    asm(".byte 0x83, 0x41, 0x00, 0x00");
    asm("mov   %%g1, %0" : "=r"(tick));
    return tick;
#elif defined(__ia64__)
    int64_t itc;
    asm("mov %0 = ar.itc" : "=r"(itc));
    return itc;
#elif defined(COMPILER_MSVC) && defined(_M_IX86)
    // Older MSVC compilers (like 7.x) don't seem to support the
    // __rdtsc intrinsic properly, so I prefer to use _asm instead
    // when I know it will work.  Otherwise, I'll use __rdtsc and hope
    // the code is being compiled with a non-ancient compiler.
    _asm rdtsc
#elif defined(COMPILER_MSVC)
    return __rdtsc();
#elif defined(__aarch64__)
    // System timer of ARMv8 runs at a different frequency than the CPU's.
    // The frequency is fixed, typically in the range 1-50MHz.  It can be
    // read at CNTFRQ special register.  We assume the OS has set up
    // the virtual timer properly.
    int64_t virtual_timer_value;
    asm volatile("mrs %0, cntvct_el0" : "=r"(virtual_timer_value));
    return virtual_timer_value;
#elif defined(__ARM_ARCH)
#if (__ARM_ARCH >= 6) // V6 is the earliest arch that has a standard cyclecount
    uint32_t pmccntr;
    uint32_t pmuseren;
    uint32_t pmcntenset;
    // Read the user mode perf monitor counter access permissions.
    asm volatile("mrc p15, 0, %0, c9, c14, 0" : "=r"(pmuseren));
    if (pmuseren & 1) { // Allows reading perfmon counters for user mode code.
        asm volatile("mrc p15, 0, %0, c9, c12, 1" : "=r"(pmcntenset));
        if (pmcntenset & 0x80000000ul) { // Is it counting?
            asm volatile("mrc p15, 0, %0, c9, c13, 0" : "=r"(pmccntr));
            // The counter is set up to count every 64th cycle
            return static_cast<int64_t>(pmccntr) * 64; // Should optimize to << 6
        }
    }
#endif
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return static_cast<int64_t>(tv.tv_sec) * 1000000 + tv.tv_usec;
#elif defined(__mips__)
    // mips apparently only allows rdtsc for superusers, so we fall
    // back to gettimeofday.  It's possible clock_gettime would be better.
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return static_cast<int64_t>(tv.tv_sec) * 1000000 + tv.tv_usec;
#else
// The soft failover to a generic implementation is automatic only for ARM.
// For other platforms the developer is expected to make an attempt to create
// a fast implementation and use generic version if nothing better is available.
#error You need to define CycleTimer for your OS and CPU
#endif
}

/**
 * measure the number of ticks per second
 *
 * @param timeout_ms \n
 *         how much time you want to use to measure, maybe a second, 5 seconds, etc.
 * @return number of ticks per second
 */
inline int64_t MeasureCyclesPerSecond(int timeout_ms) {
    int64_t t[2];
    t[0] = Now();
    handycpp::time::timer timer;
    timer.setTimeout([&t]() { t[1] = Now(); }, timeout_ms);

    while (!timer.stopped()) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(10ms);
    }
    return (t[1] - t[0]) * 1000 / timeout_ms;
}

} // namespace cycle_clock

} // namespace handycpp::time

#endif // HANDYCPP_TIME_H
