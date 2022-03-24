//
// Created by zhangfuwen on 2022/2/21.
//

#ifndef HANDYCPP_FILE_H
#define HANDYCPP_FILE_H

#include <cstring>
#include <dirent.h>
#include <fstream>
#include <functional>
#include <libgen.h>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "handycpp/logging.h"
#if __cplusplus >= 201703L
#include <filesystem>
#else
#include <sys/stat.h>
#endif

namespace handycpp::file {

static inline int removeFile(const std::string &file) { return remove(file.c_str()); }

static inline int renameFile(const std::string &old, const std::string &newfile) {
    return std::rename(old.c_str(), newfile.c_str());
}

static inline int removeDir(const std::string &dir) { return rmdir(dir.c_str()); }

static inline size_t getFileSize(const std::string &path) {
    FILE *f;
    f = fopen(path.c_str(), "r");
    int ret = fseek(f, 0, SEEK_END);
    if (ret < 0) {
        return ret;
    }
    auto len = ftell(f);
    if (len < 0) {
        return len;
    }
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
 *  lineOp(lineNumber, lineString) -> int return -1 will stop line iterating
 *  lineNumber is zero-based
 * @return
 *      return -1 on error, and errno is set
 *      return number of lines parsed
 */
static inline int for_each_line(const std::string &filePath, const std::function<int(int, std::string)> &lineOp) {
    std::ifstream input(filePath);
    if (!input.good()) {
        errno = ENOENT;
        return -1;
    }
    int i = 0;
    for (std::string line; getline(input, line);) {
        if (auto ret = lineOp(i, line); ret < 0) {
            i++;
            return i;
        }
        i++;
    }
    return i;
}

static inline bool is_file_exist(const char *fileName) {
    std::ifstream infile(fileName);
    return infile.good();
}

#if __cplusplus >= 201703L && !defined(ANDROID)
#else
#include <sys/stat.h>
#endif

static inline bool is_dir_exist(const char *dirname) {
#if __cplusplus >= 201703L && !defined(ANDROID)
    return std::filesystem::is_directory(dirname);
#else
    struct stat info;

    int statRC = stat(dirname, &info);
    if (statRC != 0) {
        if (errno == ENOENT) {
            return 0;
        } // something along the path does not exist
        if (errno == ENOTDIR) {
            return 0;
        } // something in path prefix is not a dir
        return -1;
    }

    return (info.st_mode & S_IFDIR) ? 1 : 0;
#endif
}

#include <fstream>
#include <streambuf>
#include <string>
static inline std::string readText(const std::string &path) {
    std::ifstream t(path);
    std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
    return str;
}

static inline mem_chunk readFile(const std::string &path, size_t size = 0) {
    if (size == 0) {
        size = getFileSize(path);
    }

    mem_chunk ret{};
    ret.addr = (void *)malloc(size);
    if (ret.addr == nullptr) {
        return ret;
    }
    ret.size = size;

    std::ifstream myfile;
    myfile.open(path);
    myfile.read((char *)ret.addr, (int)ret.size);
    myfile.close();

    return ret;
}

static inline std::string readTextFile(const std::string& path) {
    std::ostringstream stream;
    int ret = for_each_line(path, [&stream](int n, const std::string &line) {
        (void)n;
        stream << line;
        return 0;
    });
    if (ret < 0) {
        return "";
    }
    return stream.str();
}

#if defined(ANDROID)
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

inline static int create_dir([[maybe_unused]] const char *path, [[maybe_unused]] bool recursive = false) {
#if __cplusplus >= 201703L && !defined(ANDROID)
    std::error_code ec;
    std::filesystem::create_directories(path, ec);
    return ec.value();
#else

    struct stat st = {0};

    if (stat("/some/directory", &st) == -1) {
        mkdir("/some/directory", 0700);
    }
    return -1; // not implemented, :)
#endif
}

static inline bool saveFile(char *data, int size, const std::string &filename = "", bool createdir = false) {
    FUN_DEBUG("filepath %s", filename.c_str());
    auto filename_dump = strdup(filename.c_str());
    std::string dir(dirname(filename_dump));
    free(filename_dump);
    if (!is_dir_exist(dir.c_str()) && createdir) {
        auto ret = create_dir(dir.c_str());
        if (ret != 0) {
            return false;
        }
    }

    std::ofstream myfile;
    myfile.open(filename);
    if (!myfile.is_open()) {
        FUN_ERROR("failed to open file %s", filename.c_str());
        return false;
    }
    myfile.write(data, size);
    myfile.close();
    return true;
}

static inline void listFiles(
    const std::string &path,
    std::vector<std::string> &files,
    bool recursive = false,
    const bool showHiddenDirs = false,
    bool include_folders = false) {
    DIR *dpdf;
    struct dirent *epdf;
    dpdf = opendir(path.c_str());
    if (dpdf != nullptr) {
        while ((epdf = readdir(dpdf)) != nullptr) {
            if (showHiddenDirs
                    ? (epdf->d_type == DT_DIR && std::string(epdf->d_name) != ".." && std::string(epdf->d_name) != ".")
                    : (epdf->d_type == DT_DIR && strstr(epdf->d_name, "..") == nullptr &&
                       strstr(epdf->d_name, ".") == nullptr)) {
                if (recursive) {
                    listFiles(path + epdf->d_name + "/", files, showHiddenDirs);
                }
            }
            if (epdf->d_type == DT_DIR) {
                if (include_folders) {
                    files.push_back(path + epdf->d_name);
                }
            } else {
                files.push_back(path + epdf->d_name);
            }
        }
    }
    closedir(dpdf);
}

static inline void listFiles(const std::string &path, const std::function<void(const std::string &)> &cb) {
    if (auto dir = opendir(path.c_str())) {
        while (auto f = readdir(dir)) {
            if (f->d_name[0] == '.')
                continue;
            if (f->d_type == DT_DIR) {
                listFiles(path + f->d_name + "/", cb);
            } else {
                cb(path + f->d_name);
            }
        }
        closedir(dir);
    }
}

} // namespace handycpp::file
#endif // HANDYCPP_FILE_H
