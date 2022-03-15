//
// Created by zhangfuwen on 2022/2/21.
//

#ifndef HANDYCPP_STRING_H
#define HANDYCPP_STRING_H

#include <algorithm>
#include <climits>
#include <cmath>
#include <string>
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
[[maybe_unused]] inline std::string &trim(std::string &s) {
    ltrim(s);
    rtrim(s);
    return s;
}

// trim from start (copying)
[[maybe_unused]] inline std::string ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
[[maybe_unused]] inline std::string rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
[[maybe_unused]] inline std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

// for string delimiter
inline std::vector<std::string> split (std::string s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find (delimiter, pos_start)) != std::string::npos) {
        token = s.substr (pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back (token);
    }

    res.push_back (s.substr (pos_start));
    return res;
}

#if __cplusplus >= 202002L
static inline bool starts_with(std::string_view str, std::string_view prefix) { return str.starts_with(prefix); }
static inline bool ends_with(std::string_view str, std::string_view suffix) { return str.ends_with(suffix); }
#else
static inline bool starts_with(const std::string &str, const std::string &prefix) {
    return str.substr(0, prefix.size()) == prefix;
}
static inline bool ends_with(const std::string &str, const std::string &suffix) {
    return str.substr(str.size() - suffix.size()) == suffix;
}
#endif

/**
 * return the index of the first separator string in input.
 * @param input : input string to index
 * @param sep : separator string to find
 * @return index of the first separator string found, return std::string::npos if none is found
 */
inline std::string::size_type Index(std::string_view input, std::string_view sep);

/**
 * Join a set to strings to make a new string
 * @param input : a vector of strings
 * @param sep : separator string to be inserted between input strings
 * @param n : only join the first n input strings
 * @return joint new string
 */
[[maybe_unused]] inline std::string Join(std::vector<std::string> input, int n, std::string_view sep);

/**
 * Replace oldS into newS in input string
 * @param input : input string to replace
 * @param oldS
 * @param newS
 * @param n : only replace the first n oldS string, default value is UINT_MAX
 * @return a new string
 */
[[maybe_unused]] inline std::string
Replace(std::string_view input, std::string_view oldS, std::string_view newS, uint32_t n = UINT_MAX);

[[maybe_unused]] inline bool HasPrefix(std::string_view input, std::string_view prefix);

/**
 * Replace oldS into newS in input string, and return a new string, the original string is not changed
 * @param input : input string to replace
 * @param oldS
 * @param newS
 * @param n : only replace the first n oldS string, default value is UINT_MAX
 * @return a new string
 */
[[maybe_unused]] inline std::string
Replace(std::string_view input, std::string_view oldS, std::string_view newS, uint32_t n) {
    std::string s;
    std::string_view in = input;
    for (std::string::size_type i = 0; i < n; i++) {
        auto pos = Index(in, oldS);
        if (pos == std::string::npos) {
            s += in;
            return s;
        } else {
            s += in.substr(0, pos);
            s += newS;
            in = in.substr(pos + oldS.size());
        }
    }
    return s;
}

/**
 * Join a set to strings to make a new string
 * @param input : a vector of strings
 * @param sep : separator string to be inserted between input strings
 * @param n : only join the first n input strings
 * @return joint new string
 */
[[maybe_unused]] inline std::string
Join(std::vector<std::string> input, std::string_view sep, std::string::size_type n = 0) {
    if (n == 0) {
        n = input.size();
    }
    std::string s = input[0];

    for (std::string::size_type i = 1; i < n; i++) {
        s += sep;
        s += input[i];
    }
    return s;
}

/**
 * return a string count times and make a new string
 * @param input : string to repeat
 * @param count : times to repeat
 * @param sep : separator to be inserted between repeats
 * @return new string
 */
[[maybe_unused]] inline std::string Repeat(std::string_view input, int count, std::string_view sep = "") {
    if (count == 0) {
        return "";
    }
    std::string s(input);
    for (int i = 1; i < count; i++) {
        if (!sep.empty()) {
            s += sep;
        }
        s += input;
    }
    return s;
}

[[maybe_unused]] inline bool HasPrefix(std::string_view input, std::string_view prefix) {
    return (input.substr(0, prefix.size()).compare(prefix) == 0);
}

[[maybe_unused]] inline bool HasSuffix(std::string_view input, std::string_view suffix) {
    if (input.size() < suffix.size()) {
        return false;
    }
    if (input.size() == suffix.size()) {
        return input.compare(suffix) == 0;
    }
    return (input.substr(input.size() - suffix.size()).compare(suffix) == 0);
}

/**
 * Replace prefix with new string and return a new string, the original string is not changed
 * @param input
 * @param prefix
 * @param newStr
 * @return
 */
[[maybe_unused]] inline std::string
ReplacePrefix(std::string_view input, std::string_view prefix, std::string_view newStr) {
    if (!HasPrefix(input, prefix)) {
        return std::string(input);
    } else {
        return std::string(newStr) + std::string(input.substr(prefix.size()));
    }
}

/**
 * Replace suffix with new string and return a new string, the original string is not changed
 * @param input
 * @param suffix
 * @param newStr
 * @return
 */
[[maybe_unused]] inline std::string
ReplaceSuffix(std::string_view input, std::string_view suffix, std::string_view newStr) {
    if (!HasSuffix(input, suffix)) {
        return std::string(input);
    } else {
        return std::string(input.substr(0, input.size() - suffix.size())) + std::string(newStr);
    }
}
/**
 * return the index of the first separator string in input.
 * @param input : input string to index
 * @param sep : separator string to find
 * @return index of the first separator string found, return std::string::npos if none is found
 */
inline std::string::size_type Index(std::string_view input, std::string_view sep) {
    for (std::string::size_type i = 0; i < input.size(); i++) {
        if (input.substr(i, sep.size()).compare(sep) == 0) {
            return i;
        }
    }
    return std::string::npos;
}

/**
 * return the index of the first char in input.
 * @param input : input string to index
 * @param cutset : chars to find
 * @return index of the first char found
 */
[[maybe_unused]] inline std::string_view::size_type IndexAny(std::string_view input, std::string_view cutset) {
    return input.find_first_of(cutset);
}

/**
 * split input string into substrings
 * @param input : input string to split
 * @param cutset : a set of chars that can act as a separator
 * @param N : maximum number of substrings to return
 * @return
 */
[[maybe_unused]] inline std::vector<std::string>
SplitAny(std::string_view input, const std::string_view &cutset, uint32_t N = UINT_MAX) {

    std::vector<std::string> ret;
    if (N == 0) {
        return ret;
    }
    if (N == 1) {
        ret.resize(1);
        ret[0] = input;
        return ret;
    }

    if (cutset.empty()) {
        uint32_t maxSize = std::min((uint32_t)input.size(), N);
        ret.resize(maxSize);
        for (std::string::size_type i = 0; i < maxSize; i++) {
            if (i == maxSize - 1) {
                ret[i] = input.substr(i);
            } else {
                ret[i] = input[i];
            }
        }
        return ret;
    }

    std::string::size_type start = input.find_first_not_of(cutset);
    std::string_view in = input.substr(start);

    for (std::string::size_type i = 0; i < N - 1; i++) {
        if (in.empty()) {
            return ret;
        }
        std::string::size_type cur = in.find_first_of(cutset);
        if (cur == std::string::npos) {
            ret.emplace_back(in);
            return ret;
        } else {
            ret.emplace_back(input.substr(0, cur));
            in = in.substr(cur);
            in = in.substr(in.find_first_not_of(cutset));
        }
    }
    return ret;
}

/**
 * split input into a set of substrings
 * @param input : input string to split
 * @param sep : separator string
 * @param N : maximum number of substrings to return
 * @return
 */
[[maybe_unused]] inline std::vector<std::string> Split(std::string input, const std::string &sep, uint32_t N = UINT_MAX) {
    std::vector<std::string> ret;
    if (sep.empty()) {
        ret.resize(input.size());
        for (std::string::size_type i = 0; i < input.size(); i++) {
            ret[i] = std::string("") + input[i];
        }
        return ret;
    }

    std::string_view in = input;
    if (in.substr(0, sep.size()).compare(sep) == 0) {
        in = in.substr(sep.size());
    }

    for (std::string::size_type i = 0; i < N; i++) {
        if (in.empty()) {
            return ret;
        }
        std::string::size_type cur = Index(in, sep);
        if (cur == std::string::npos) {
            ret.emplace_back(in);
            return ret;
        } else {
            ret.emplace_back(in.substr(0, cur));
            in = in.substr(cur + sep.size());
        }
    }
    return ret;
}

[[maybe_unused]] inline std::string ToLower(std::string_view input) {
    std::string ret;
    ret.resize(input.size());
    for (std::string::size_type i = 0; i < input.size(); i++) {
        ret[i] = (char)std::tolower(input[i]);
    }
    return ret;
}

[[maybe_unused]] inline std::string ToUpper(std::string_view input) {
    std::string ret;
    ret.resize(input.size());
    for (std::string::size_type i = 0; i < input.size(); i++) {
        ret[i] = (char)std::toupper(input[i]);
    }
    return ret;
}

/**
 * remove characters in cutset from both left and right of input
 * @param input : string to trim
 * @param cutset : a set of character to remove
 * @return trimmed string
 */
[[maybe_unused]] inline std::string_view Trim(std::string_view input, std::string_view cutset) {
    std::string::size_type first = input.find_first_not_of(cutset);
    std::string::size_type last = input.find_last_not_of(cutset);
    if (last < first) {
        return "";
    }
    return input.substr(first, last - first + 1);
}

/**
 * remove characters in cutset from left
 * @param input : string to trim
 * @param cutset : a set of character to remove
 * @return trimmed string
 */
inline std::string_view TrimLeft(std::string_view input, std::string_view cutset) {
    return input.substr(input.find_first_not_of(cutset));
}

/**
 * remove characters in cutset from left
 * @param input : string to trim
 * @param func : function to test if a char should be removed
 * @return trimmed string
 */
[[maybe_unused]] inline std::string_view TrimLeftFunc(std::string_view input, const std::function<bool(char)> &func) {
    std::string::size_type first = 0;
    for (; first < input.size(); first++) {
        if (!func(input[first])) {
            break;
        }
    }
    return input.substr(first);
}

/**
 * remove characters in cutset from and right
 * @param input : string to trim
 * @param cutset : a set of character to remove
 * @return trimmed string
 */
inline std::string_view TrimRight(std::string_view input, std::string_view cutset) {
    std::string::size_type last = input.find_last_not_of(cutset);
    return input.substr(0, last + 1);
}

/**
 * Trim chars from right until the first char that make func return false
 * @param input : string to trim
 * @param func : func to tell whether a char need to be trimmed
 * @return trimmed string
 */
[[maybe_unused]] inline std::string_view TrimRightFunc(std::string_view input, const std::function<bool(char)> &func) {
    int last = (int)input.size();
    for (; last >= 0; last--) {
        if (!func(input[last - 1])) {
            break;
        }
    }
    return input.substr(0, last);
}

/**
 * Trim prefix from a string
 * @param input : string to trim
 * @param prefix : prefix
 * @return trimmed string
 */
[[maybe_unused]] inline std::string_view TrimPrefix(std::string_view input, std::string_view prefix) {
    for (unsigned int i = 0; i < prefix.length(); i++) {
        if (input[i] != prefix[i]) {
            return input;
        }
    }
    return input.substr(prefix.size());
}

/**
 * Trim suffix strings
 * @param input : string to trim
 * @param suffix : suffix
 * @return trimmed string
 */
[[maybe_unused]] inline std::string_view TrimSuffix(std::string_view input, std::string_view suffix) {
    int last = (int)input.size() - 1;
    std::string::size_type suffix_last = suffix.size() - 1;
    for (unsigned int i = 0; i < suffix.length() && (last - (int)i) >= 0; i++) {
        if (input[last - i] != suffix[suffix_last - i]) {
            return input;
        }
    }
    return input.substr(0, input.size() - suffix.size());
}

/**
 * Trim white spaces(\\t\\v\\r\\n\\f) from both ends
 * @param input: the string to trim
 * @return trimmed string
 */
[[maybe_unused]] inline std::string_view TrimSpace(std::string_view input) {
    // https://en.cppreference.com/w/cpp/string/byte/isspace
    const std::string spaceChars = " \t\v\n\r\f";
    std::string_view ret = TrimLeft(input, spaceChars);
    return TrimRight(ret, spaceChars);
}

} // namespace handycpp::string

#endif // HANDYCPP_STRING_H
