//
// Created by zhangfuwen on 2022/2/21.
//

#include <cassert>
#include "handycpp/file.h"
#include "handycpp/logging.h"
#include "handycpp/string.h"
#include "handycpp/exec.h"
#include "handycpp/image.h"
#include "handycpp/dyntype.h"
#include <string>

using namespace handycpp::dyntype::concat;
using namespace handycpp::dyntype::arithmetic;
using namespace std;

int main() {
//    auto chunk = handycpp::file::readFile("/data1/XR/NDK_OpenGLES_3_0/2048_2048.rgb");
//    if(chunk.size != 0) {
////        handycpp::image::writeBmp("/data1/XR/NDK_OpenGLES_3_0/2048_2048.png", (unsigned char *)chunk.addr, 2048, 2048, 3);
//        handycpp::image::saveRgbaToPng("/data1/XR/NDK_OpenGLES_3_0/2048_2048.png", (unsigned char *)chunk.addr, 2048, 2048);
//    } else {
//        printf("failed to read");
//    }
//    printf("%lu\n", __cplusplus);
    std::string s = "hello world";
    assert(1 == handycpp::string::starts_with(s, "hello"));
    assert(1 == handycpp::string::ends_with(s, "world"));
    assert(0 == handycpp::string::starts_with(s, "ello"));
    assert(0 == handycpp::string::ends_with(s, "worl"));
    std::string s1 = "2";
    assert(7 == 5 +  s1);
    assert(3 == 5 -  s1);
    assert(7 == (long long)5 +  s1);
    std::string s2 = "4.1";
    assert(5.1f == 1.0f + s2);
    assert((double)5.1 == (double)1.0f + s2);

    std::string hi = "hi";
    assert("hi6" == hi + 6 );
    assert("hi6" == hi + 6L );

    assert(  0 + s1 - 5 == -3);
    assert(  0.0f + s1 - 5.0 == -3.0);

    using namespace handycpp::dyntype::implicit_conversion;
    assert( (int)(sw)s1 + 3 == 5);
    using namespace handycpp::dyntype::arithmetic;
    assert( 0 + s1 + 3 == 5);
    return 0;
}
