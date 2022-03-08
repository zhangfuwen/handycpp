# handycpp

handy(maybe also buggy) C++ helper functions.


## dyntype


concat everything to string:
```cpp
using namespace handycpp::dyntype::concat;
std::string hi = "hi";
assert("hi6" == hi + 6 );
assert("hi6" == hi + 6L );
```

arithmetic operation with string:
```cpp
using namespace handycpp::dyntype::arithmetic;
std::string s1 = "2";
assert(7 == 5 +  s1);
assert(3 == 5 -  s1);
assert(7 == (long long)5 +  s1);
std::string s2 = "4.1";
assert(5.1f == 1.0f + s2);
assert((double)5.1 == (double)1.0f + s2);

// if you want to use string in the first place, you use zero to add string
assert(  0 + s1 - 5 == -3);
assert(  0.0f + s1 - 5.0 == -3.0);

// or use implicit_conversion::sw(not handy at the moment)
using namespace handycpp::dyntype::implicit_conversion;
assert( (int)(sw)s1 + 3 == 5);
using namespace handycpp::dyntype::arithmetic;
assert( 0 + s1 + 3 == 5);
```
