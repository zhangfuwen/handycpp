//
// Created by zhangfuwen on 2022/2/21.
//

#ifndef HANDYCPP_FILE_H
#define HANDYCPP_FILE_H

#include <string>
#include <fstream>
#include <cstring>
#include <dirent.h>
#include <vector>
#include <cstring>
#include <string>
#include <functional>

#include "handycpp/logging.h"

namespace handycpp::file {

static inline int removeFile(std::string file) {
    return remove(file.c_str());
}

static inline int renameFile(std::string old, std::string newfile) {
    return std::rename(old.c_str(), newfile.c_str());
}

static inline int removeDir(std::string dir) {
    return rmdir(dir.c_str());
}


static inline size_t getFileSize(const std::string& path)
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

/**
 * for_each_line
 * @param filePath
 * @param lineOp
//  lineOp(lineNumber, lineString)
//  lineNumber is zero-based
 * @return
 *      return -1 on error, and errno is set
 *      return number of lines parsed
 */
static inline int for_each_line(std::string filePath, std::function<void(int, std::string)> lineOp) {
    std::ifstream input(filePath);
    if(!input.good()) {
        errno = ENOENT;
        return -1;
    }
    int i = 0;
    for( std::string line; getline( input, line ); )
    {
        lineOp(i, line);
        i++;
    }
    return i;
}

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
    if(!myfile.is_open()) {
        FUN_ERROR("failed to open file %s", filename.c_str());
        return false;
    }
    myfile.write(data, size);
    myfile.close();
    return true;
}

static inline bool is_file_exist(const char *fileName)
{
    std::ifstream infile(fileName);
    return infile.good();
}

static inline void listFiles(const std::string& path, std::vector<std::string> &files, bool recursive = false, const bool showHiddenDirs = false, bool include_folders=false){
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir(path.c_str());
    if (dpdf != NULL){
        while ((epdf = readdir(dpdf)) != NULL){
            if(showHiddenDirs ? (epdf->d_type==DT_DIR && std::string(epdf->d_name) != ".." && std::string(epdf->d_name) != "." ) : (epdf->d_type==DT_DIR && strstr(epdf->d_name,"..") == NULL && strstr(epdf->d_name,".") == NULL ) ){
                if(recursive) {
                    listFiles(path+epdf->d_name+"/",files, showHiddenDirs);
                }
            }
            if(epdf->d_type == DT_DIR ) {
                if(include_folders) {
                    files.push_back(path+epdf->d_name);
                }
            } else {
                files.push_back(path+epdf->d_name);
            }
        }
    }
    closedir(dpdf);
}

static inline void listFiles(const std::string &path, std::function<void(const std::string &)> cb) {
    if (auto dir = opendir(path.c_str())) {
        while (auto f = readdir(dir)) {
            if (!f->d_name || f->d_name[0] == '.') continue;
            if (f->d_type == DT_DIR)  {
                listFiles(path + f->d_name + "/", cb);
            } else {
                cb(path + f->d_name);
            }
        }
        closedir(dir);
    }
}

}
#endif // HANDYCPP_FILE_H
