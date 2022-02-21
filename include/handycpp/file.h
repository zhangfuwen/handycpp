//
// Created by zhangfuwen on 2022/2/21.
//

#ifndef HANDYCPP_FILE_H
#define HANDYCPP_FILE_H

#include <string>
#include <fstream>
#include "handycpp/logging.h"

static inline bool saveFile(char *data, int size, const std::string& filename = "") {
  FUN_DEBUG("filepath %s", filename.c_str());
  std::ofstream myfile;
  myfile.open (filename);
  myfile.write(data, size);
  myfile.close();
  return true;
}
#endif // HANDYCPP_FILE_H
