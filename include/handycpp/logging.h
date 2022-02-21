//
// Created by zhangfuwen on 2022/2/21.
//

#ifndef HANDYCPP_LOGGING_H
#define HANDYCPP_LOGGING_H

#include <string_view>
#include <signal.h>
#include <stdlib.h>
#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string>
#include <filesystem>
#include <syslog.h>

constexpr auto trim_filename(std::string_view path)
{
  return path.substr(path.find_last_of("/\\") + 1);
}

#define g_debug printf

#define FUN_INFO(fmt, ...)                                                     \
    do {                                                                       \
        auto tid = gettid();                                                   \
        auto pid = getpid();                                                   \
        g_info("%d %d %s:%d %s > " fmt, pid, tid, trim_filename(__FILE__).data(),          \
           __LINE__, __FUNCTION__, ##__VA_ARGS__);                        \
    } while (0)

#define FUN_ERROR(fmt, ...)                                                     \
    do {                                                                       \
        auto tid = gettid();                                                   \
        auto pid = getpid();                                                   \
        g_info("%d %d %s:%d %s > " fmt, pid, tid, trim_filename(__FILE__).data(),          \
           __LINE__, __FUNCTION__, ##__VA_ARGS__);                        \
    } while (0)

#define FUN_DEBUG(fmt, ...)                                                     \
    do {                                                                       \
        auto tid = gettid();                                                   \
        auto pid = getpid();                                                   \
        g_debug("%d %d %s:%d %s > " fmt, pid, tid, trim_filename(__FILE__).data(),          \
           __LINE__, __FUNCTION__, ##__VA_ARGS__);                        \
    } while (0)

#define FUN_WARN(fmt, ...)                                                     \
    do {                                                                       \
        char buff[40]; \
        auto tid = gettid();                                                   \
        auto pid = getpid();                                                   \
        g_warning("%d %d %s:%d %s > " fmt, pid, tid, trim_filename(__FILE__).data(),          \
           __LINE__, __FUNCTION__, ##__VA_ARGS__);                        \
    } while (0)

#define FUN_TRACE(fmt, ...)                                                     \
    do {                                                                       \
        auto tid = gettid();                                                   \
        auto pid = getpid();                                                   \
        g_debug("%d %d %s:%d %s > " fmt, pid, tid, trim_filename(__FILE__).data(),          \
           __LINE__, __FUNCTION__, ##__VA_ARGS__);                        \
    } while (0)

#ifdef NDEBUG
#undef FUN_TRACE
#undef FUN_DEBUG
#define FUN_DEBUG(fmt, ...)
#define FUN_TRACE(fmt, ...)
#endif

#endif // HANDYCPP_LOGGING_H
