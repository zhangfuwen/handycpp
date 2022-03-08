# handycpp


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
```
