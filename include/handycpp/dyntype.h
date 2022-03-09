//
// Created by zhangfuwen on 2022/3/9.
//

#ifndef HANDYCPP_DYNTYPE_H
#define HANDYCPP_DYNTYPE_H

#include <sstream>
#include <string>
#include <utility>
namespace handycpp::dyntype {

template <class T> inline std::string to_string(const T &t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

namespace implicit_conversion {

// string wrapper, for implicit conversion
struct sw {
public:
    sw(const std::string &s) { val = s; }

    operator float() { return std::stof(val); }

    operator double() { return std::stod(val); }

    operator long double() { return std::stold(val); }

    operator int() { return std::stoi(val); }
    operator unsigned int() { return std::stoul(val); }

    operator long() { return std::stol(val); }
    operator unsigned long() { return std::stoul(val); }

    operator long long() { return std::stoll(val); }

    operator unsigned long long() { return std::stoull(val); }

private:
    std::string val;
};

} // namespace implicit_conversion

namespace concat {
inline std::string operator+(const std::string &s, char i) { return s + to_string(i); }
inline std::string operator+(const std::string &s, unsigned char i) { return s + to_string(i); }
inline std::string operator+(const std::string &s, short i) { return s + to_string(i); }
inline std::string operator+(const std::string &s, unsigned short i) { return s + to_string(i); }

inline std::string operator+(const std::string &s, int i) { return s + to_string(i); }
inline std::string operator+(const std::string &s, unsigned int i) { return s + to_string(i); }

inline std::string operator+(const std::string &s, long i) { return s + to_string(i); }

inline std::string operator+(const std::string &s, unsigned long i) { return s + to_string(i); }
inline std::string operator+(const std::string &s, long long i) { return s + to_string(i); }

inline std::string operator+(const std::string &s, unsigned long long i) { return s + to_string(i); }

inline std::string operator+(const std::string &s, float i) { return s + to_string(i); }
inline std::string operator+(const std::string &s, double i) { return s + to_string(i); }
inline std::string operator+(const std::string &s, long double i) { return s + to_string(i); }
template <typename T> std::enable_if_t<!std::is_arithmetic_v<T>, std::string> operator+(const std::string &s, T i) {
    return s + to_string(i);
}

} // namespace concat

namespace arithmetic {
inline int operator+(char i, const std::string &s) { return i + (char)std::stoi(s); }
inline unsigned int operator+(unsigned char i, const std::string &s) { return i + std::stoi(s); }
inline int operator+(short i, const std::string &s) { return i + std::stoi(s); }
inline unsigned int operator+(unsigned short i, const std::string &s) { return i + std::stoi(s); }

inline long long operator+(int i, const std::string &s) { return i + std::stoll(s); }
inline unsigned int operator+(unsigned int i, const std::string &s) { return i + std::stoll(s); }
inline long operator+(long i, const std::string &s) { return i + std::stoll(s); }
inline unsigned long operator+(unsigned long i, const std::string &s) { return i + std::stoll(s); }

inline long long operator+(long long i, const std::string &s) { return i + std::stoll(s); }
inline unsigned long long operator+(unsigned long long i, const std::string &s) { return i + std::stoull(s); }
inline float operator+(float i, const std::string &s) { return i + std::stof(s); }
inline double operator+(double i, const std::string &s) { return i + std::stod(s); }
inline long double operator+(long double i, const std::string &s) { return i + std::stold(s); }

inline int operator-(char i, const std::string &s) { return i - std::stoi(s); }
inline unsigned char operator-(unsigned char i, const std::string &s) { return i - std::stoi(s); }
inline int operator-(short i, const std::string &s) { return i - std::stoi(s); }
inline unsigned short operator-(unsigned short i, const std::string &s) { return i - std::stoi(s); }

inline long long operator-(int i, const std::string &s) { return i - std::stoll(s); }
inline unsigned int operator-(unsigned int i, const std::string &s) { return i - std::stoll(s); }
inline long operator-(long i, const std::string &s) { return i - std::stoll(s); }
inline unsigned long operator-(unsigned long i, const std::string &s) { return i - std::stoll(s); }

inline long long operator-(long long i, const std::string &s) { return i - std::stoll(s); }
inline unsigned long long operator-(unsigned long long i, const std::string &s) { return i - std::stoull(s); }
inline float operator-(float i, const std::string &s) { return i - std::stof(s); }
inline double operator-(double i, const std::string &s) { return i - std::stod(s); }
inline long double operator-(long double i, const std::string &s) { return i - std::stold(s); }
inline int operator*(char i, const std::string &s) { return i * std::stoi(s); }
inline unsigned char operator*(unsigned char i, const std::string &s) { return i * std::stoi(s); }
inline int operator*(short i, const std::string &s) { return i * std::stoi(s); }
inline unsigned short operator*(unsigned short i, const std::string &s) { return i * std::stoi(s); }

inline long long operator*(int i, const std::string &s) { return i * std::stoll(s); }
inline unsigned int operator*(unsigned int i, const std::string &s) { return i * std::stoll(s); }
inline long operator*(long i, const std::string &s) { return i * std::stoll(s); }
inline unsigned long operator*(unsigned long i, const std::string &s) { return i * std::stoll(s); }

inline long long operator*(long long i, const std::string &s) { return i * std::stoll(s); }
inline unsigned long long operator*(unsigned long long i, const std::string &s) { return i * std::stoull(s); }
inline float operator*(float i, const std::string &s) { return i * std::stof(s); }
inline double operator*(double i, const std::string &s) { return i * std::stod(s); }
inline long double operator*(long double i, const std::string &s) { return i * std::stold(s); }
inline int operator/(char i, const std::string &s) { return i / std::stoi(s); }
inline unsigned int operator/(unsigned char i, const std::string &s) { return i / std::stoi(s); }
inline int operator/(short i, const std::string &s) { return i / std::stoi(s); }
inline unsigned int operator/(unsigned short i, const std::string &s) { return i / std::stoi(s); }

inline long long operator/(int i, const std::string &s) { return i / std::stoll(s); }
inline unsigned int operator/(unsigned int i, const std::string &s) { return i / std::stoll(s); }
inline long operator/(long i, const std::string &s) { return i / std::stoll(s); }
inline unsigned long operator/(unsigned long i, const std::string &s) { return i / std::stoll(s); }

inline long long operator/(long long i, const std::string &s) { return i / std::stoll(s); }
inline unsigned long long operator/(unsigned long long i, const std::string &s) { return i / std::stoull(s); }
inline float operator/(float i, const std::string &s) { return i / std::stof(s); }
inline double operator/(double i, const std::string &s) { return i / std::stod(s); }
inline long double operator/(long double i, const std::string &s) { return i / std::stold(s); }
} // namespace arithmetic

} // namespace handycpp::dyntype

#endif // HANDYCPP_DYNTYPE_H
