//
// Created by Administrator on 2022/10/22.
//

#ifndef HANDYCPP_SIGNAL_H
#define HANDYCPP_SIGNAL_H
#include <functional>
#include <map>

template <typename... Args>
class Signal {

public:
    Signal() = default;
    ~Signal() = default;

    // Copy constructor and assignment create a new signal.
    Signal(Signal const & /*unused*/) {}

    Signal &operator=(Signal const &other) {
        if (this != &other) {
            disconnect_all();
        }
        return *this;
    }

    // Move constructor and assignment operator work as expected.
    Signal(Signal &&other) noexcept : _slots(std::move(other._slots)), _current_id(other._current_id) {}

    Signal &operator=(Signal &&other) noexcept {
        if (this != &other) {
            _slots = std::move(other._slots);
            _current_id = other._current_id;
        }

        return *this;
    }

    // Connects a std::function to the signal. The returned
    // value can be used to disconnect the function again.
    int connect(std::function<void(Args...)> const &slot) const {
        _slots.insert(std::make_pair(++_current_id, slot));
        return _current_id;
    }

    // Convenience method to connect a member function of an
    // object to this Signal.
    template <typename T> int connect_member(T *inst, void (T::*func)(Args...)) {
        return connect([=](Args... args) { (inst->*func)(args...); });
    }

    // Convenience method to connect a const member function
    // of an object to this Signal.
    template <typename T> int connect_member(T *inst, void (T::*func)(Args...) const) {
        return connect([=](Args... args) { (inst->*func)(args...); });
    }

#if defined(HANDYCPP_EVENT_LOOP_H)
    // Connects a std::function to the signal. The returned
    // value can be used to disconnect the function again.
    int connect(std::function<void(Args...)> const &slot, EventLoop * loop) const {
        _slots.insert(std::make_pair(++_current_id, [=](Args... args) {
            [[maybe_unused]] auto ret = loop->enqueueAsync(slot, std::forward<Args>(args)...);
        }));
        return _current_id;
    }

    // Convenience method to connect a member function of an
    // object to this Signal.
    template <typename T> int connect_member(T *inst, void (T::*func)(Args...), EventLoop * loop) {
        return connect([=](Args... args) { (inst->*func)(std::forward<Args>(args)...); }, loop);
    }

    // Convenience method to connect a const member function
    // of an object to this Signal.
    template <typename T> int connect_member(T *inst, void (T::*func)(Args...) const, EventLoop *loop) {
        return connect([=](Args... args) { (inst->*func)(std::forward<Args>(args)...); }, loop);
    }
#endif

    // Disconnects a previously connected function.
    void disconnect(int id) const { _slots.erase(id); }

    // Disconnects all previously connected functions.
    void disconnect_all() const { _slots.clear(); }

    // Calls all connected functions.
    void emit(Args... p) {
        for (auto const &it : _slots) {
            it.second(std::forward<Args>(p)...);
        }
    }

    // Calls all connected functions except for one.
    void emit_for_all_but_one(int excludedConnectionID, Args... p) {
        for (auto const &it : _slots) {
            if (it.first != excludedConnectionID) {
                it.second(std::forward<Args>(p)...);
            }
        }
    }

    // Calls only one connected function.
    void emit_for(int connectionID, Args... p) {
        auto const &it = _slots.find(connectionID);
        if (it != _slots.end()) {
            it->second(std::forward<Args>(p)...);
        }
    }

private:
    mutable std::map<int, std::function<void(Args...)>> _slots;
    mutable int _current_id{0};
};
#ifdef HANDYCPP_TEST
class MyThread : public EventLoop {
public:
    int ret;
    void add(int a, int b) {
        ret = a+b + gettid();
    }
};
TEST_CASE("handycpp::signal_slop") {
    Signal<int, int> signal;
    int ret;
    std::unique_ptr<MyThread> loop = std::make_unique<MyThread>();

    signal.connect([&](int a, int b) {
        ret=a+b;
    });
    signal.connect_member(loop.get(), &MyThread::add, loop.get());

    signal.emit(1, 4);
    auto tid = loop->enqueueSync([]() {
#if defined(__linux__)
        return (int)get_tid();
#elif defined(_WIN32)
        return (int)GetCurrentThreadId();
#else
        return 0;
#endif
    });

    CHECK((ret == 5));
    CHECK((loop->ret == tid + 5));
}
class MyThread2 : public EventLoop {
public:
    int ret;
    void add(std::string & a, int * b) {
        std::cout << "xxx" << std::endl;
        *b = EventLoop::gettid();
        a[0] = 'x';
        std::cout << "xxx end " << a[0] << std::endl;
    }
    void test(std::string &a) {
        a[0] = 'x';
        printf("e %p\n", (void*)&a[0]);
    }
};
TEST_CASE("handycpp::signal_slop_const_ref") {
    Signal<std::string &, int *> signal;
    int ret;
    std::unique_ptr<MyThread2> loop = std::make_unique<MyThread2>();

    signal.connect([](std::string & a, int *b) {
        std::cout << "yyy" << std::endl;
        *b = 5;
        a[0] ='y';
      std::cout << "yyy end " << a[0] << std::endl;
    });

    std::string s = "hello";
    signal.emit(s, &ret);
    CHECK((ret == 5));
    CHECK((s[0] == 'y'));

    signal.connect_member(loop.get(), &MyThread2::add, loop.get());
    signal.emit(s, &ret);

    auto tid = loop->enqueueSync([]() {
#if defined(__linux__)
        return (int)get_tid();
#elif defined(_WIN32)
        return (int)GetCurrentThreadId();
#else
        return 0;
#endif
    });

    CHECK((ret == tid));
    std::cout << s[0] << std::endl;
    CHECK((s[0] == 'x'));

    Signal<std::string & > signal2;
    std::string s1 = "y";
    printf("a %p\n", (void*)s1.data());
    signal2.connect_member(loop.get(), &MyThread2::test, loop.get());
    signal2.emit(s1);
    loop->enqueueSync([](){});
    CHECK_EQ(s1[0], 'x');

}


#endif
#endif // HANDYCPP_SIGNAL_H
