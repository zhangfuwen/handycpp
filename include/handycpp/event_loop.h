//
// Created by Administrator on 2022/10/22.
//

#ifndef HANDYCPP_EVENT_LOOP_H
#define HANDYCPP_EVENT_LOOP_H
#include <condition_variable>
#include <functional>
#include <future>
#include <thread>
#include <vector>

/**
 * event loop is ran in a thread,
 * thread is created in constructor and stop and waited on destructor
 *
 * for late initialization, try using a std::unique_ptr and another class to hold event loop
 *
 * @example
 * \code{.cpp}
 * class A {
 * public:
 *     void start() {
 *         m_loop = std::make_unique<EventLoop>();
 *     }
 *     void stop() {
 *         m_loop = nullptr;
 *     }
 * private:
 *     std::unique_ptr<EventLoop> m_loop = nullptr;
 * }
 * // and when you want to start the loop
 * loop = std::make_unique<EventLoop>();
 * // when you want to stop loop thread:
 * loop = nullptr;
 * \endcode
 */
class EventLoop {
public:
    using callable_t = std::function<void()>;

    EventLoop() = default;
    EventLoop(const EventLoop &) = delete;
    EventLoop(EventLoop &&) noexcept = delete;
    ~EventLoop() noexcept {
        enqueue([this] { m_running = false; });
        m_thread.join();
    }

    EventLoop &operator=(const EventLoop &) = delete;
    EventLoop &operator=(EventLoop &&) noexcept = delete;

    void enqueue(callable_t &&callable) noexcept {
        {
            std::lock_guard<std::mutex> guard(m_mutex);
            m_writeBuffer.emplace_back(std::move(callable));
        }
        m_condVar.notify_one();
    }

    /**
     * enqueueSync
     *  pass by reference supported
     * @tparam Func
     * @tparam Args
     * @param callable
     * @param args
     * @return
     * @example
     * \code{.cpp}
     * std::cout << eventLoop.enqueueSync([](const int& x, int&& y, int z)
     * {
     *     return x + y + z;
     * }, 1, 2, 3);
     * \endcode
     */
    template <typename Ret, typename... Args>
    auto enqueueSync(std::function<Ret(Args...)> &&callable, Args &&...args) {
        if (std::this_thread::get_id() == m_thread.get_id()) {
            return std::invoke(std::forward<std::function<Ret(Args...)>>(callable), std::forward<Args>(args)...);
        }

        using return_type = std::invoke_result_t<std::function<Ret(Args...)>, Args...>;
        using packaged_task_type = std::packaged_task<return_type(Args && ...)>;

        packaged_task_type task(std::forward<std::function<Ret(Args...)>>(callable));

        enqueue([&] { task(std::forward<Args>(args)...); });

        return task.get_future().get();
    }

    /**
     * pass by reference not supported
     * @tparam Func
     * @tparam Args
     * @param callable
     * @param args
     * @return
     */
    template<typename Func, typename... Args>
    auto enqueueSync(Func&& callable, Args&& ...args)
    {
        if (std::this_thread::get_id() == m_thread.get_id())
        {
            return std::invoke(
                std::forward<Func>(callable),
                std::forward<Args>(args)...);
        }

        using return_type = std::invoke_result_t<Func, Args...>;
        using packaged_task_type =
            std::packaged_task<return_type(Args&&...)>;

        packaged_task_type task(std::forward<Func>(callable));

        enqueue([&]
                {
                    task(std::forward<Args>(args)...);
                });

        return task.get_future().get();
    }

    /**
     * enqueueAsync
     * @tparam Func
     * @tparam Args
     * @param callable
     * @param args
     * @return std::future
     * @example
     * \code{.cpp}
     * std::future<int> result = eventLoop.enqueueAsync([](int x, int y)
     * {
     *    return x + y;
     * }, 1, 2);
     * //
     * //do some heavy work here
     * //
     * std::cout << result.get();
     * \endcode
     */
    template<typename Func, typename... Args>
    [[nodiscard]] auto enqueueAsync(Func&& callable, Args&& ...args)
    {
        using return_type = std::invoke_result_t<Func, Args...>;
        using packaged_task_type = std::packaged_task<return_type()>;

        auto taskPtr = std::make_shared<packaged_task_type>(std::bind(
            std::forward<Func>(callable), std::forward<Args>(args)...));

        enqueue(std::bind(&packaged_task_type::operator(), taskPtr));

        return taskPtr->get_future();
    }

    template<typename Ret, typename... Args>
    [[nodiscard]] auto enqueueAsync(std::function<Ret(Args...)>& callable, Args&& ...args)
    {
        using return_type = std::invoke_result_t<std::function<Ret(Args...)>, Args...>;
        using packaged_task_type = std::packaged_task<return_type()>;

        auto taskPtr = std::make_shared<packaged_task_type>(std::bind(
            std::forward<std::function<Ret(Args...)>>(callable), std::forward<Args>(args)...));

        enqueue(std::bind(&packaged_task_type::operator(), taskPtr));

        return taskPtr->get_future();
    }


    int gettid() {
       return tid;
    }
    int getpid() {
        return pid;
    }

private:
    std::vector<callable_t> m_writeBuffer;
    std::mutex m_mutex{};
    std::condition_variable m_condVar{};

    bool m_running{true};
    std::thread m_thread{&EventLoop::threadFunc, this};

    int tid;
    int pid;

    void threadFunc() noexcept {
        std::vector<callable_t> readBuffer;
#if defined(__linux__)
        tid = (int)get_tid();
        pid = (int)get_pid();
#elif defined(_WIN32)
        tid = (int)GetCurrentThreadId();
        pid = (int)GetCurrentProcessId();
#else
#endif

        while (m_running) {
            {
                std::unique_lock<std::mutex> lock(m_mutex);
                m_condVar.wait(lock, [this] { return !m_writeBuffer.empty(); });
                std::swap(readBuffer, m_writeBuffer);
            }

            for (callable_t &func : readBuffer) {
                func();
            }

            readBuffer.clear();
        }
    }
};

#ifdef HANDYCPP_TEST
#include "doctest/doctest.h"
#endif

#ifdef HANDYCPP_TEST
TEST_CASE("handycpp::event_loop") {
    std::unique_ptr<EventLoop> loop = std::make_unique<EventLoop>();

    std::function<int(int, int)> f = [](int x, int y) -> int {
        return x+ y;
    };
    std::future result = loop->enqueueAsync(f, 1, 2);
    CHECK((result.get() == 3));


    int a = 2 , b = 3;
    std::future result1 = loop->enqueueAsync([&]() { a=b; });
    result1.wait();
    CHECK_EQ(a, b);
}
#endif

#endif // HANDYCPP_EVENT_LOOP_H
