//
// Created by zhangfuwen on 2022/2/21.
//

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
    return 0;
}
