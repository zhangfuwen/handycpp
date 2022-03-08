//
// Created by zhangfuwen on 2022/3/9.
//

#ifndef HANDYCPP_DYNTYPE_H
#define HANDYCPP_DYNTYPE_H

#include <sstream>
#include <string>
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
        sw(std::string s) {
            val = s;
        }

        operator float() {
            return std::stof(val);
        }

        operator double() {
            return std::stod(val);
        }

        operator long double() {
            return std::stold(val);
        }

        operator int() {
            return std::stoi(val);
        }
        operator unsigned int() {
            return std::stoul(val);
        }

        operator long() {
            return std::stol(val);
        }
        operator unsigned long() {
            return std::stoul(val);
        }

        operator long long() {
            return std::stoll(val);
        }

        operator unsigned long long() {
            return std::stoull(val);
        }

    private:
        std::string val;
    };



}


namespace concat {
std::string operator+(std::string s, char i) { return s + to_string(i); }
std::string operator+(std::string s, unsigned char i) { return s + to_string(i); }
std::string operator+(std::string s, short i) { return s + to_string(i); }
std::string operator+(std::string s, unsigned short i) { return s + to_string(i); }

std::string operator+(std::string s, int i) { return s + to_string(i); }
std::string operator+(std::string s, unsigned int i) { return s + to_string(i); }

std::string operator+(std::string s, long i) { return s + to_string(i); }

std::string operator+(std::string s, unsigned long i) { return s + to_string(i); }
std::string operator+(std::string s, long long i) { return s + to_string(i); }

std::string operator+(std::string s, unsigned long long i) { return s + to_string(i); }

std::string operator+(std::string s, float i) { return s + to_string(i); }
std::string operator+(std::string s, double i) { return s + to_string(i); }
std::string operator+(std::string s, long double i) { return s + to_string(i); }
template <typename T> std::enable_if_t<!std::is_arithmetic_v<T>, std::string> operator+(std::string s, T i) {
    return s + to_string(i);
}

} // namespace concat

namespace arithmetic {
char operator+(char i, std::string s) { return i + std::stoi(s.c_str()); }
unsigned char operator+(unsigned char i, std::string s) { return i + std::stoi(s.c_str()); }
short operator+(short i, std::string s) { return i + std::stoi(s.c_str()); }
unsigned short operator+(unsigned short i, std::string s) { return i + std::stoi(s.c_str()); }

int operator+(int i, std::string s) { return i + std::stoll(s.c_str()); }
unsigned int operator+(unsigned int i, std::string s) { return i + std::stoll(s.c_str()); }
long operator+(long i, std::string s) { return i + std::stoll(s.c_str()); }
unsigned long operator+(unsigned long i, std::string s) { return i + std::stoll(s.c_str()); }

long long operator+(long long i, std::string s) { return i + std::stoll(s.c_str()); }
unsigned long long operator+(unsigned long long i, std::string s) { return i + std::stoull(s.c_str()); }
float operator+(float i, std::string s) { return i + std::stof(s.c_str()); }
double operator+(double i, std::string s) { return i + std::stod(s.c_str()); }
long double operator+(long double i, std::string s) { return i + std::stold(s.c_str()); }

char operator-(char i, std::string s) { return i - std::stoi(s.c_str()); }
unsigned char operator-(unsigned char i, std::string s) { return i - std::stoi(s.c_str()); }
short operator-(short i, std::string s) { return i - std::stoi(s.c_str()); }
unsigned short operator-(unsigned short i, std::string s) { return i - std::stoi(s.c_str()); }

int operator-(int i, std::string s) { return i - std::stoll(s.c_str()); }
unsigned int operator-(unsigned int i, std::string s) { return i - std::stoll(s.c_str()); }
long operator-(long i, std::string s) { return i - std::stoll(s.c_str()); }
unsigned long operator-(unsigned long i, std::string s) { return i - std::stoll(s.c_str()); }

long long operator-(long long i, std::string s) { return i - std::stoll(s.c_str()); }
unsigned long long operator-(unsigned long long i, std::string s) { return i - std::stoull(s.c_str()); }
float operator-(float i, std::string s) { return i - std::stof(s.c_str()); }
double operator-(double i, std::string s) { return i - std::stod(s.c_str()); }
long double operator-(long double i, std::string s) { return i - std::stold(s.c_str()); }
char operator*(char i, std::string s) { return i * std::stoi(s.c_str()); }
unsigned char operator*(unsigned char i, std::string s) { return i * std::stoi(s.c_str()); }
short operator*(short i, std::string s) { return i * std::stoi(s.c_str()); }
unsigned short operator*(unsigned short i, std::string s) { return i * std::stoi(s.c_str()); }

int operator*(int i, std::string s) { return i * std::stoll(s.c_str()); }
unsigned int operator*(unsigned int i, std::string s) { return i * std::stoll(s.c_str()); }
long operator*(long i, std::string s) { return i * std::stoll(s.c_str()); }
unsigned long operator*(unsigned long i, std::string s) { return i * std::stoll(s.c_str()); }

long long operator*(long long i, std::string s) { return i * std::stoll(s.c_str()); }
unsigned long long operator*(unsigned long long i, std::string s) { return i * std::stoull(s.c_str()); }
float operator*(float i, std::string s) { return i * std::stof(s.c_str()); }
double operator*(double i, std::string s) { return i * std::stod(s.c_str()); }
long double operator*(long double i, std::string s) { return i * std::stold(s.c_str()); }
char operator/(char i, std::string s) { return i / std::stoi(s.c_str()); }
unsigned char operator/(unsigned char i, std::string s) { return i / std::stoi(s.c_str()); }
short operator/(short i, std::string s) { return i / std::stoi(s.c_str()); }
unsigned short operator/(unsigned short i, std::string s) { return i / std::stoi(s.c_str()); }

int operator/(int i, std::string s) { return i / std::stoll(s.c_str()); }
unsigned int operator/(unsigned int i, std::string s) { return i / std::stoll(s.c_str()); }
long operator/(long i, std::string s) { return i / std::stoll(s.c_str()); }
unsigned long operator/(unsigned long i, std::string s) { return i / std::stoll(s.c_str()); }

long long operator/(long long i, std::string s) { return i / std::stoll(s.c_str()); }
unsigned long long operator/(unsigned long long i, std::string s) { return i / std::stoull(s.c_str()); }
float operator/(float i, std::string s) { return i / std::stof(s.c_str()); }
double operator/(double i, std::string s) { return i / std::stod(s.c_str()); }
long double operator/(long double i, std::string s) { return i / std::stold(s.c_str()); }

} // namespace arithmetic

} // namespace handycpp::dyntype

#endif // HANDYCPP_DYNTYPE_H
