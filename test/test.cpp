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
    auto chunk = handycpp::file::readFile("/data1/XR/ndk-samples/hello-gl2/124.rgba");
    if(chunk.size != 0) {
        handycpp::image::writeBmp("/data1/XR/ahardware_buffer/1.bmp", (unsigned char *)chunk.addr, 720, 1237, 4);
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
