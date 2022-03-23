//
// Created by zhangfuwen on 2022/2/21.
//

#ifndef HANDYCPP_EXEC_H
#define HANDYCPP_EXEC_H
#include <array>
#include <cstdio>
#include <memory>
#include <string>

static inline std::string exec(const std::string & cmd) {
    return exec(cmd.c_str());
}

static inline std::string exec(const char *cmd) {
    std::array<char, 128> buffer{};
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}
#endif // HANDYCPP_EXEC_H
