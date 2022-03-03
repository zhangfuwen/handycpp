//
// Created by zhangfuwen on 2022/2/21.
//

#include <cassert>
#include "handycpp/file.h"
#include "handycpp/logging.h"
#include "handycpp/string.h"
#include "handycpp/exec.h"
#include "handycpp/image.h"


int main() {
    auto chunk = handycpp::file::readFile("/data1/XR/NDK_OpenGLES_3_0/2048_2048.rgb");
    if(chunk.size != 0) {
//        handycpp::image::writeBmp("/data1/XR/NDK_OpenGLES_3_0/2048_2048.png", (unsigned char *)chunk.addr, 2048, 2048, 3);
        handycpp::image::saveRgbaToPng("/data1/XR/NDK_OpenGLES_3_0/2048_2048.png", (unsigned char *)chunk.addr, 2048, 2048);
    } else {
        printf("failed to read");
    }
    printf("%lu\n", __cplusplus);
    std::string s = "hello world";
    assert(1 == handycpp::string::starts_with(s, "hello"));
    assert(1 == handycpp::string::ends_with(s, "world"));
    assert(0 == handycpp::string::starts_with(s, "ello"));
    assert(0 == handycpp::string::ends_with(s, "worl"));
    return 0;
}
