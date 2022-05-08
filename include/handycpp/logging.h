//
// Created by zhangfuwen on 2022/2/21.
//

#ifndef HANDYCPP_LOGGING_H
#define HANDYCPP_LOGGING_H

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <filesystem>
#include <string>
#include <string_view>
#include <syslog.h>
#include <unistd.h>
#include <utility>

#if __cplusplus >= 201703L
constexpr std::string_view trim_filename(std::string_view path) { return path.substr(path.find_last_of("/\\") + 1); }
#else
static inline std::string trim_filename(std::string path) { return path.substr(path.find_last_of("/\\") + 1); }
#endif

namespace handycpp::logging {

using LogWriterFunc = std::function<void(int, const char *, const char *)>; // level, tag, message
struct LoggerChain {
    LogWriterFunc write = nullptr;
    struct LoggerChain *next = nullptr;
};

template <typename... Args> std::string log_format(const std::string &format, Args... args) {
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    if (size_s <= 0) {
        throw std::runtime_error("Error during formatting.");
    }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string{buf.get(), buf.get() + size - 1}; // We don't want the '\0' inside
}

namespace { // hidden
inline int g_level = 0;
inline LogWriterFunc g_logWrite = [](int level, const char *tag, const char *text) { printf("%s\n", text); };
} // namespace
/**
 * set log writer
 * @param func void (int level, const char * tag, const char *text)
 */
[[maybe_unused]] inline void SetLogWritter(LogWriterFunc func) { g_logWrite = std::move(func); }
[[maybe_unused]] inline void SetLogLevel(int level) { g_level = level; };
} // end namespace handycpp::logging

#ifndef FUN_PRINT
#define FUN_PRINT(fmt, ...) g_logWrite(0, "", log_format(fmt, __VA_ARGS__))
#endif

#define FUN_INFO(fmt, ...)                                                                                             \
    do {                                                                                                               \
        auto tid = gettid();                                                                                           \
        auto pid = getpid();                                                                                           \
        FUN_PRINT(                                                                                                     \
            "%d %d info %s:%d %s > " fmt,                                                                              \
            pid,                                                                                                       \
            tid,                                                                                                       \
            trim_filename(__FILE__).data(),                                                                            \
            __LINE__,                                                                                                  \
            __FUNCTION__,                                                                                              \
            ##__VA_ARGS__);                                                                                            \
    } while (0)

#define FUN_ERROR(fmt, ...)                                                                                            \
    do {                                                                                                               \
        auto tid = gettid();                                                                                           \
        auto pid = getpid();                                                                                           \
        FUN_PRINT(                                                                                                     \
            "%d %d error %s:%d %s > " fmt,                                                                             \
            pid,                                                                                                       \
            tid,                                                                                                       \
            trim_filename(__FILE__).data(),                                                                            \
            __LINE__,                                                                                                  \
            __FUNCTION__,                                                                                              \
            ##__VA_ARGS__);                                                                                            \
    } while (0)

#define FUN_DEBUG(fmt, ...)                                                                                            \
    do {                                                                                                               \
        auto tid = gettid();                                                                                           \
        auto pid = getpid();                                                                                           \
        FUN_PRINT(                                                                                                     \
            "%d %d debug %s:%d %s > " fmt,                                                                             \
            pid,                                                                                                       \
            tid,                                                                                                       \
            trim_filename(__FILE__).data(),                                                                            \
            __LINE__,                                                                                                  \
            __FUNCTION__,                                                                                              \
            ##__VA_ARGS__);                                                                                            \
    } while (0)

#define FUN_WARN(fmt, ...)                                                                                             \
    do {                                                                                                               \
        char buff[40];                                                                                                 \
        auto tid = gettid();                                                                                           \
        auto pid = getpid();                                                                                           \
        FUN_PRINT(                                                                                                     \
            "%d %d warning %s:%d %s > " fmt,                                                                           \
            pid,                                                                                                       \
            tid,                                                                                                       \
            trim_filename(__FILE__).data(),                                                                            \
            __LINE__,                                                                                                  \
            __FUNCTION__,                                                                                              \
            ##__VA_ARGS__);                                                                                            \
    } while (0)

#define FUN_TRACE(fmt, ...)                                                                                            \
    do {                                                                                                               \
        auto tid = gettid();                                                                                           \
        auto pid = getpid();                                                                                           \
        FUN_PRINT(                                                                                                     \
            "%d %d trace %s:%d %s > " fmt,                                                                             \
            pid,                                                                                                       \
            tid,                                                                                                       \
            trim_filename(__FILE__).data(),                                                                            \
            __LINE__,                                                                                                  \
            __FUNCTION__,                                                                                              \
            ##__VA_ARGS__);                                                                                            \
    } while (0)

#ifdef NDEBUG
#undef FUN_TRACE
#undef FUN_DEBUG
#define FUN_DEBUG(fmt, ...)
#define FUN_TRACE(fmt, ...)
#endif

#endif // HANDYCPP_LOGGING_H
