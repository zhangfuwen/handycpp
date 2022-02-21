//
// Created by zhangfuwen on 2022/2/21.
//

#ifndef HANDYCPP_FILE_H
#define HANDYCPP_FILE_H

#include <string>
#include <fstream>
#include <cstring>
#include "handycpp/logging.h"
namespace handycpp::file {
static inline auto getFileSize(const std::string& path)
{
    FILE *f;
    f = fopen(path.c_str() , "r");
    int ret = fseek(f, 0, SEEK_END);
    size_t len = (unsigned long)ftell(f);
    fclose(f);
    return len;
}

struct mem_chunk {
    void *addr;
    size_t size;
};

static inline mem_chunk readFile(std::string path, size_t size = 0) {
    if(size == 0) {
        size = getFileSize(path);
    }

    mem_chunk ret{};
    ret.addr = (void*)malloc(size);
    if(ret.addr == nullptr) {
        return ret;
    }
    ret.size = size;

    std::ifstream myfile;
    myfile.open(path);
    myfile.read((char*)ret.addr, ret.size);
    myfile.close();

    return ret;
}

static inline bool saveFile(char *data, int size, const std::string& filename = "") {
    FUN_DEBUG("filepath %s", filename.c_str());
    std::ofstream myfile;
    myfile.open (filename);
    myfile.write(data, size);
    myfile.close();
    return true;
}

}
#endif // HANDYCPP_FILE_H
