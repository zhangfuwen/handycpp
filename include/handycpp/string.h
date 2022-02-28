//
// Created by zhangfuwen on 2022/2/21.
//

#ifndef HANDYCPP_STRING_H
#define HANDYCPP_STRING_H

#include <string>
#include <algorithm>
namespace handycpp::string {

// trim from start (in place)
static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    return s;
}

// trim from end (in place)
static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), s.end());
    return s;
}

// trim from both ends (in place)
static inline std::string &trim(std::string &s) {
    ltrim(s);
    rtrim(s);
    return s;
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

#if __cplusplus >= 202002L
static inline bool starts_with(std::string_view str, std::string_view prefix) {
    return str.starts_with(prefix);
}
static inline bool ends_with(std::string_view str, std::string_view suffix) {
    return str.ends_with(suffix);
}
#else
static inline bool starts_with(const std::string &str, const std::string &prefix) {
    return str.substr(0, prefix.size()) == prefix;
}
static inline bool ends_with(const std::string &str, const std::string &suffix) {
    return str.substr(str.size() - suffix.size()) == suffix;
}
#endif
};

#endif // HANDYCPP_STRING_H
