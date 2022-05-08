//
// Created by zhangfuwen on 2022/2/21.
//

#ifndef HANDYCPP_STRING_H
#define HANDYCPP_STRING_H

#include <algorithm>
#include <climits>
#include <cmath>
#include <string>
#include <optional>
#include <regex>
#include <utility>
#include <set>
#include <sstream>
#include <memory>
#include <string>
#include <stdexcept>

#ifdef HANDYCPP_TEST
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"
#endif

namespace handycpp::string {

/**
 * print to string using printf syntax.
 *
 * @usage
 *        Example:\n\n
 *              std::string s = format("%s:%d %s", __FILE__, __LINE__, __func__);
 * @tparam Args
 * @param format
 * @param args
 * @return return formated string
 */
template<typename ... Args>
std::string format( const std::string& fmt, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, fmt.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, fmt.c_str(), args ... );
    auto ret = std::string{ buf.get(), buf.get() + size - 1 }; // We don't want the '\0' inside
    return ret;
}

template<>
std::string format(const std::string &fmt)
{
    return fmt;
}

#ifdef HANDYCPP_TEST
TEST_CASE("handycpp::string::format") {
    auto ret  = format("a%c %d %d %.3f", 'a', 1, 2,  3.567000);
    using namespace std::string_literals;
    CHECK(ret == "aa 1 2 3.567"s);
    CHECK("ok"s == format("ok"));
    CHECK(format("").empty());
}
#endif

/**
 * test if a string contains another string
 * @param self
 * @param anotherStr
 * @return
 */
static inline bool Contains(const std::string &self, const std::string & anotherStr) {
    return self.find(anotherStr) != std::string::npos;
}

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
SplitAny(std::string_view input, const std::string_view &cutset = " ", uint32_t N = UINT_MAX) {

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
            ret.emplace_back(in.substr(0, cur));
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

namespace pipe_operator {

/********************** split like *************************************/
class pipe_functor_split {
public:
    virtual std::vector<std::string> operator()(const std::string &) = 0;
    virtual ~pipe_functor_split() = default;
};

inline static std::vector<std::string>
operator|(const std::string &input, std::function<std::vector<std::string>(const std::string &)> &f) {
    return f(input);
}
inline static std::vector<std::string>
operator|(const std::string &input, std::function<std::vector<std::string>(const std::string &)> &&f) {
    return f(input);
}

inline static std::vector<std::string> operator|(const std::string &input, pipe_functor_split &f) { return f(input); }

class split_functor : public pipe_functor_split {
public:
    std::vector<std::string> operator()(const std::string &input) override {
        return handycpp::string::SplitAny(input, " \t\v");
    }
};

class [[maybe_unused]] splitby: public pipe_functor_split {
public:
    splitby(std::string cutset) : cutset_(cutset) {}
    std::vector<std::string> operator()(const std::string &input) override {
        return handycpp::string::SplitAny(input, cutset_);
    }
private:
    std::string cutset_;
};

inline splitby split_(" ");
inline std::function<std::vector<std::string>(const std::string &)> split = split_;

/************************************ join like ********************************************/
class pipe_functor_join {
public:
    virtual std::string operator()(const std::vector<std::string> &) = 0;
    virtual ~pipe_functor_join() = default;
};
inline static std::string
operator|(const std::vector<std::string> &input, std::function<std::string(const std::vector<std::string> &)> &f) {
    return f(input);
}

inline static std::string
operator|(const std::vector<std::string> &input, std::function<std::string(const std::vector<std::string> &)> &&f) {
    return f(input);
}

inline static std::string operator|(const std::vector<std::string> &input, pipe_functor_join &f) { return f(input); }

class join_functor : public pipe_functor_join {
public:
    std::string operator()(const std::vector<std::string> &input) override { return handycpp::string::Join(input, ""); }
};

template <char cut> class joinby_functor : public pipe_functor_join {
public:
    std::string operator()(const std::vector<std::string> &input) override {
        using namespace std::string_literals;
        std::string s;
        s.push_back(cut);
        return handycpp::string::Join(input, s);
    }
};
inline join_functor join;

inline std::function<std::string(const std::vector<std::string> &inputs)> joinby(const std::string &sep) {
    return [&sep](const std::vector<std::string> &inputs) -> std::string {
        std::stringstream ret;
        auto it = inputs.begin();
        ret << *it;
        it++;
        while (it != inputs.end()) {
            ret << sep << *it;
            it++;
        }
        return ret.str();
    };
}

/**************************** filter like *********************************/

class pipe_functor_filter {
public:
    virtual std::vector<std::string> operator()(const std::vector<std::string> &) = 0;
    virtual ~pipe_functor_filter() = default;
};

inline static std::vector<std::string> operator|(
    std::vector<std::string> &&input,
    std::function<std::vector<std::string>(const std::vector<std::string> &)> &f) {
    return f(input);
}
inline static std::vector<std::string> operator|(
    std::vector<std::string> &&input,
    std::function<std::vector<std::string>(const std::vector<std::string> &)> &&f) {
    return f(input);
}
inline static std::vector<std::string> operator|(
    const std::vector<std::string> &input,
    std::function<std::vector<std::string>(const std::vector<std::string> &)> &f) {
    return f(input);
}
inline static std::vector<std::string> operator|(
    const std::vector<std::string> &input,
    std::function<std::vector<std::string>(const std::vector<std::string> &)> &&f) {
    return f(input);
}

class filter_out : public pipe_functor_filter {
public:
    explicit filter_out(const std::string &s) { strs.insert(s); }
    explicit filter_out(std::set<std::string> &strSet) { strs.swap(strSet); }
    std::vector<std::string> operator()(const std::vector<std::string> &inputs) override {
        std::vector<std::string> ret;
        for (const auto &word : inputs) {
            if (!strs.count(word)) {
                ret.emplace_back(word);
            }
        }
        return ret;
    }

private:
    std::set<std::string> strs;
};

class foreach : public pipe_functor_filter {
public:
    explicit foreach(std::function<std::string(const std::string &)> f) : _f(std::move(f)) {}

    std::vector<std::string> operator()(const std::vector<std::string> &inputs) override {
        std::vector<std::string> ret;
        for (const auto &word : inputs) {
            ret.emplace_back(_f(word));
        }
        return ret;
    }

private:
    std::function<std::string(const std::string &)> _f;
};


template<typename T>
struct memfun_type2
{
    using type = void;
    using input_type = void;
};

template<typename Ret, typename Input>
struct memfun_type2<std::optional<Ret>(*)(const Input &)>
{
    using type = std::function<Ret(Input)>;
    using ret_type = Ret;
    using input_type = Input;
};

template<typename Ret, typename Class, typename Input>
struct memfun_type2<std::optional<Ret>(Class::*)(const Input &) const>
{
    using type = std::function<Ret(Input)>;
    using ret_type = Ret;
    using input_type = Input;
    using class_type = Class;
};

template<typename Ret, typename Input>
struct memfun_type2<std::function<std::optional<Ret>(const Input &)>>
{
    using type = std::function<std::optional<Ret>(const Input &)>;
    using ret_type = Ret;
    using input_type = Input;
};

template<typename Ret, typename Class, typename Input>
struct memfun_type2<std::optional<Ret>(Class::*)(const Input &)>
{
    using type = std::function<Ret(Input)>;
    using ret_type = Ret;
    using input_type = Input;
    using class_type = Class;
};


template<typename F, typename Input = typename memfun_type2<decltype(&F::operator())>::input_type,
    typename T = typename memfun_type2<decltype(&F::operator())>::ret_type
>
inline auto collect(F &&func) -> std::function<std::vector<T>(const std::vector<Input> & inputs)> {
    return [func](const std::vector<Input> & inputs) -> std::vector<T> {
        std::vector<T> ret;
        for(const auto &input : inputs) {
            std::optional<T> res = func(input);
            if(res.has_value()) {
                ret.push_back(res.value());
            }
        }
        return ret;
    };
}

template<typename Input, typename T>
inline auto collect(std::function<std::optional<T>(const Input &)> func) -> std::function<std::vector<T>(const std::vector<Input> & inputs)> {
    return [func](const std::vector<Input> & inputs) -> std::vector<T> {
      std::vector<T> ret;
      for(const auto &input : inputs) {
          std::optional<T> res = func(input);
          if(res.has_value()) {
              ret.push_back(res.value());
          }
      }
      return ret;
    };
}

template<typename F, typename Input = typename memfun_type2<F*>::input_type,
    typename T = typename memfun_type2<F*>::ret_type
>
inline auto collect(F *func) -> std::function<std::vector<T>(const std::vector<Input> & inputs)> {
    return [func](const std::vector<Input> & inputs) -> std::vector<T> {
      std::vector<T> ret;
      for(const auto &input : inputs) {
          std::optional<T> res = func(input);
          if(res.has_value()) {
              ret.push_back(res.value());
          }
      }
      return ret;
    };
}

#ifdef HANDYCPP_TEST
TEST_CASE("testing filter") {
    using namespace handycpp::string::pipe_operator;
    using namespace std::string_literals;
    auto s = "hello world dean"s;
    auto res = s | split;
    auto ret = res | filter_out("world");
    CHECK(ret.size() == 2);
    CHECK(ret[0] == "hello");
    CHECK(ret[1] == "dean");

    ret = s | split | [](const std::vector<std::string> & inputs) -> auto {
      std::vector<std::string> ret;
      for(const auto & word : inputs) {
          ret.push_back("sss"s + word);
      }
      return ret;
    };
    CHECK(ret.size() == 3);
    CHECK(ret[0] == "ssshello");
    CHECK(ret[1] == "sssworld");
    CHECK(ret[2] == "sssdean");

    ret = s | split | collect([](const std::string & word) -> std::optional<std::string> {
              if(word.find("l") != word.npos) {
                  return word;
              }
              return std::nullopt;
    });
    CHECK(ret.size() == 2);
    CHECK(ret[0] == "hello");
    CHECK(ret[1] == "world");
}
#endif

/************************************* convert like *******************************************/

class pipe_functor_convert {
public:
    virtual std::string operator()(const std::string &) = 0;
    virtual ~pipe_functor_convert() = default;
};
inline static std::string operator|(const std::string &input, std::function<std::string(const std::string &)> &f) {
    return f(input);
}

inline static std::string operator|(const std::string &input, std::function<std::string(const std::string &)> &&f) {
    return f(input);
}

inline static std::string operator|(const std::string &input, pipe_functor_convert &&f) {
    return f(input);
}

class append: public pipe_functor_convert {
public:
    [[maybe_unused]] explicit append(std::string suffix): suffix_(std::move(suffix)) {}

    std::string operator() (const std::string & input) override {
        return input + suffix_;
    }

private:
    std::string suffix_;
};

class prepend: public pipe_functor_convert {
public:
    [[maybe_unused]] explicit prepend(std::string prefix): prefix_(std::move(prefix)) {}

    std::string operator() (const std::string & input) override {
        return prefix_ + input;
    }

private:
    std::string prefix_;
};

#ifdef HANDYCPP_TEST
TEST_CASE("testing pipe_convert") {
    using namespace handycpp::string::pipe_operator;
    using namespace std::string_literals;
    auto ret = "hello world dean"s | split |  foreach(prepend("sss"));
    CHECK(ret.size() == 3);
    CHECK(ret[0] == "ssshello");
    CHECK(ret[1] == "sssworld");
    CHECK(ret[2] == "sssdean");

    auto s = "hello world dean"s;
    ret = s | split | foreach(append("sss"));
    CHECK(ret.size() == 3);
    CHECK(ret[0] == "hellosss");
    CHECK(ret[1] == "worldsss");
    CHECK(ret[2] == "deansss");

}

#endif

/********************************** test like ******************************************/

class pipe_functor_test {
public:
    virtual bool operator()(const std::string &) = 0;
    virtual ~pipe_functor_test() = default;
};

inline std::function<bool(const std::string &)> not_true(std::function<bool(const std::string &)> f) {
    return [&f](const std::string & input) {
        return !f(input);
    };
}

template <typename Input, typename F, typename T = std::result_of_t<F&(Input)>>
inline static
T operator|(Input input, F &f) {
    return f(input);
}

template <typename Input, typename F, typename T = std::result_of_t<F&(Input)>>
inline static
T operator|(Input input, F &&f) {
    return f(input);
}

template <typename T>
inline static T operator|(const std::vector<std::string> &inputs, std::function<T(const std::string &)> &f) {
    return std::all_of(inputs.begin(), inputs.end(), f);
}

template <typename T>
inline static T operator|(const std::vector<std::string> &inputs, std::function<T(const std::string &)> &&f) {
    return std::all_of(inputs.begin(), inputs.end(), f);
}

template<typename T>
struct memfun_type
{
    using type = void;
};

template<typename Ret, typename Class, typename... Args>
struct memfun_type<Ret(Class::*)(Args...) const>
{
    using type = std::function<Ret(Args...)>;
};

template<typename F>
typename memfun_type<decltype(&F::operator())>::type
to_function(F const &func)
{ // Function from lambda !
    return func;
}

template<typename T>
struct memfun_type1
{
    using type = void;
    using input_type = void;
};

template<typename Ret, typename Input>
struct memfun_type1<Ret(*)(const Input &)>
{
    using type = std::function<Ret(Input)>;
    using ret_type = Ret;
    using input_type = Input;
};

template<typename Ret, typename Class, typename Input>
struct memfun_type1<Ret(Class::*)(const Input &) const>
{
    using type = std::function<Ret(Input)>;
    using ret_type = Ret;
    using input_type = Input;
    using class_type = Class;
};
template<typename Ret, typename Class, typename Input>
struct memfun_type1<Ret(Class::*)(const Input &)>
{
    using type = std::function<Ret(Input)>;
    using ret_type = Ret;
    using input_type = Input;
    using class_type = Class;
};

template<typename F, typename Input = typename memfun_type1<decltype(&F::operator())>::input_type,
          typename T = typename memfun_type1<decltype(&F::operator())>::ret_type
          >
inline auto all(F &&func) -> std::function<T(const std::vector<Input> & inputs)> {
    return [func](const std::vector<Input> & inputs) -> T {
      return std::all_of(inputs.begin(), inputs.end(), func);
    };
}

template<typename F, typename Input = typename memfun_type1<F*>::input_type,
    typename T = typename memfun_type1<F*>::ret_type
>
inline auto all(F *func) -> std::function<T(const std::vector<Input> & inputs)> {
    return [func](const std::vector<Input> & inputs) -> T {
      return std::all_of(inputs.begin(), inputs.end(), func);
    };
}

template<typename F, typename Input = typename memfun_type1<decltype(&F::operator())>::input_type,
    typename T = typename memfun_type1<decltype(&F::operator())>::ret_type
>
inline auto any(F &&func) -> std::function<T(const std::vector<Input> & inputs)> {
    return [func](const std::vector<Input> & inputs) -> T {
      return std::any_of(inputs.begin(), inputs.end(), func);
    };
}

template<typename F, typename Input = typename memfun_type1<F*>::input_type,
    typename T = typename memfun_type1<F*>::ret_type
>
inline auto any(F *func) -> std::function<T(const std::vector<Input> & inputs)> {
    return [func](const std::vector<Input> & inputs) -> T {
      return std::any_of(inputs.begin(), inputs.end(), func);
    };
}





#ifdef HANDYCPP_TEST

bool test_word_size(const std::string & word) {
    return word.size() >= 4;
}
TEST_CASE("testing pipe_operator") {
    using namespace handycpp::string::pipe_operator;
    using namespace std::string_literals;
    auto s = "hello world dean"s;
    auto ret = s | [](const std::string &word) { return word.empty(); };
    CHECK(ret == false);
    ret = s | [](const std::string &word) -> bool { return word.empty(); };
    CHECK(ret == false);

    ret = s | [](auto word) { return word.empty(); };
    CHECK(ret == false);

    ret = s | split | all(to_function([](const std::string & word) { return word.size() == 5;}));
    CHECK(ret == false);

  auto f1 = all([](const std::string & word)-> bool { return word.size() >= 4;});
    ret = s | split |  f1;
    CHECK(ret == true);

    ret = s | split | all([](const std::string & word)-> bool { return word.size() >= 4;});
    CHECK(ret == true);

    std::string capture="cap";
    ret = s | split | all([&capture](const std::string & )-> bool { return capture.size() >= 4;});
    CHECK(ret == false);

    class functor {
    public:
        bool operator()(const std::string & word) {
            return word.size() >= 4;
        }
    };
    ret = s | split | all(functor());
    CHECK(ret == true);

    ret = s | split | all(test_word_size);
    CHECK(ret == true);

    decltype(test_word_size) * fptr = test_word_size;
    ret = s | split | all(fptr);
    CHECK(ret == true);

    ret = s | split | any(not_true(fptr));
    CHECK(ret == false);
}
#endif

/******************************************* regex ****************************************/

[[maybe_unused]] inline std::function<std::optional<std::string>(const std::string &)> grep(const std::string &substr) {
    return [&substr](const std::string &input) -> std::optional<std::string> {
        if (handycpp::string::Contains(input, substr)) {
            return input;
        } else {
            return std::nullopt;
        }
    };
}

[[maybe_unused]] std::function<std::optional<std::string>(const std::string &)>
inline egrep(const std::string &regexStr, bool onlyMatch = false) {
    return [regexStr, onlyMatch](const std::string &input) -> std::optional<std::string> {
        std::smatch sm;
        if (std::regex_search(input, sm, std::regex(regexStr))) {
            if (onlyMatch) {
                return sm[0];
            } else {
                return input;
            }
        }
        return std::nullopt;
    };
}

[[maybe_unused]] std::function<std::vector<std::string>(const std::string &)>
inline egrep_submatch(const std::string &regexStr) {
    return [regexStr](const std::string &input) -> std::vector<std::string> {
      std::smatch sm;
      std::vector<std::string> submatch;
      if (std::regex_search(input, sm, std::regex(regexStr))) {
            for(const auto & i : sm) {
                submatch.push_back(i);
            }
      }
      return submatch;
    };
}

#ifdef HANDYCPP_TEST
TEST_CASE("testing regex") {
    using namespace handycpp::string::pipe_operator;
    using namespace std::string_literals;
    {
        auto s = "hello world dean"s;
        auto ret = s | grep("world");
        CHECK(ret.has_value() == true);
        CHECK(ret.value() == s);

        s = "hello well hell ok";
        auto ret1 = s | split | collect(grep("ll"));
        CHECK(ret1.size() == 3);
        CHECK(ret1[0] == "hello");
        CHECK(ret1[1] == "well");
        CHECK(ret1[2] == "hell");

        auto ret2 = s | split | collect(grep("ll")) | joinby(",");
        CHECK(ret2 == "hello,well,hell");

    }

    {
        auto test = [](std::string ip, std::string s= "") {
            std::string str;
            if(s.empty()) {
                str = "(\\d{1,3})\\."
                           "(\\d{1,3})\\."
                           "(\\d{1,3})\\."
                           "(\\d{1,3})";

            } else {
                str = s;
            }
          auto ret = ip | egrep(str);
          CHECK(ret.has_value());
          CHECK(ret.value() == ip);

          auto ret1 = ip | egrep_submatch(str);
          CHECK(ret1.size() == 5);
          CHECK(ret1[0] == "192.168.1.10");
          CHECK(ret1[1] == "192");
          CHECK(ret1[2] == "168");
            CHECK(ret1[3] == "1");
            CHECK(ret1[4] == "10");
        };
        auto ip = "192.168.1.10";
        auto ip2 = "xxad192.168.1.10as";
        auto ip1 = "aasdf192.168.1.10ads192.168.1.11asd";
        test(ip);
        test(ip2);
        test(ip1);

        auto ip3 = "aasdf192.168.1.10ads192.168.2.11asd";
        auto x = ip3 | egrep_submatch("^aa.*?(\\d{1,3})\\." // ? means non-gready here
                  "(\\d{1,3})\\."
                  "(\\d{1,3})\\."
                  "(\\d{1,3})"
                  "(.*$)");
        CHECK(x.size() == 6);
        CHECK(x[1] == "192");
        CHECK(x[2] == "168");
        CHECK(x[3] == "1");
        CHECK(x[4] == "10");
    }
}
#endif

} // namespace handycpp::string::pipe_operator

} // namespace handycpp::string
#ifdef HANDYCPP_TEST
TEST_CASE("testing pipe_operator") {
    using namespace handycpp::string::pipe_operator;
    using namespace std::string_literals;
    auto s = "hello world dean"s;
    auto words = "hello world"s | split;
    CHECK(words[0] == "hello");
    CHECK(words[1] == "world");

    words = s | split;
    CHECK(words[0] == "hello");
    CHECK(words[1] == "world");
    CHECK(words[2] == "dean");

    auto line = words | join;
    CHECK(line == "helloworlddean");
    auto line2 = words | joinby_functor<','>();
    CHECK(line2 == "hello,world,dean");
    auto line3 = words | joinby(",,");
    CHECK(line3 == "hello,,world,,dean");


}
#endif
#endif // HANDYCPP_STRING_H
