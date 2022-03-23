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
    void setTimeout(T function, int delay_ms);
    template<typename T>
    void setInterval(T function, int inverval_ms);

    template<typename T, typename Rep, typename Period>
    void setTimeout(T function, std::chrono::duration<Rep, Period> delay);
    template<typename T, typename Rep, typename Period>
    void setInterval(T function, std::chrono::duration<Rep, Period> inverval);
    void stop();

};

template<typename T>
void timer::setTimeout(T function, int delay_ms) {
    this->clear = false;
    std::thread t([=]() {
        if (this->clear) return;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_ms));
        if (this->clear) return;
        function();
    });
    t.detach();
}

template<typename T>
void timer::setInterval(T function, int inverval_ms) {
    this->clear = false;
    std::thread t([=]() {
        while (true) {
            if (this->clear) return;
            std::this_thread::sleep_for(std::chrono::milliseconds(inverval_ms));
            if (this->clear) return;
            function();
        }
    });
    t.detach();
}

template<typename T, typename Rep, typename Period>
void timer::setTimeout(T function, std::chrono::duration<Rep, Period> delay) {
    this->clear = false;
    std::thread t([=]() {
      if (this->clear) return;
      std::this_thread::sleep_for(delay);
      if (this->clear) return;
      function();
    });
    t.detach();
}

template<typename T, typename Rep, typename Period>
void timer::setInterval(T function, std::chrono::duration<Rep, Period> inverval) {
    this->clear = false;
    std::thread t([=]() {
      while (true) {
          if (this->clear) return;
          std::this_thread::sleep_for(std::chrono::milliseconds(inverval));
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