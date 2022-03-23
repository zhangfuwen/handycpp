//
// Created by zhangfuwen on 2022/3/23.
//

#ifndef HANDYCPP_TIME_H
#define HANDYCPP_TIME_H

#include <iostream>
#include <thread>
#include <chrono>

namespace handycpp::time {

class timer {
    volatile bool clear = false;

public:
    template<typename T>
    void setTimeout(T function, int delay);
    template<typename T>
    void setInterval(T function, int interval);
    void stop();

};

template<typename T>
void timer::setTimeout(T function, int delay) {
    this->clear = false;
    std::thread t([=]() {
        if (this->clear) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        if (this->clear) return;
        function();
    });
    t.detach();
}

template<typename T>
void timer::setInterval(T function, int interval) {
    this->clear = false;
    std::thread t([=]() {
        while (true) {
            if (this->clear) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(interval));
            if (this->clear) return;
            function();
        }
    });
    t.detach();
}

inline void timer::stop() {
    this->clear = true;
}

}

#endif // HANDYCPP_TIME_H
