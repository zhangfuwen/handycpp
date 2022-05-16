//
// Created by zhangfuwen on 2022/5/16.
//

#ifndef HANDYCPP_HUMAN_READABLE_H
#define HANDYCPP_HUMAN_READABLE_H

#include <string>
#include <cstdio>

namespace handycpp::human_readable {
inline std::string to_string_2(double x) {
    char a[20];
    sprintf(a, "%.2f", x);
    return a;
}

inline std::string hr(uint64_t val) { // human readable
    if (val / 1000000000 != 0) {
        auto ret = double(val) / 1000000000;
        return to_string_2(ret) + " G";
    } else if (val / 1000000 != 0) {
        auto ret = double(val) / 1000000;
        return to_string_2(ret) + " M";
    } else if (val / 1000 != 0) {
        auto ret = double(val) / 1000;
        return to_string_2(ret) + " M";
    } else {
        return std::to_string(val);
    }
}

}

#endif // HANDYCPP_HUMAN_READABLE_H
