//
// Created by zhangfuwen on 2022/3/1.
//

#include "lodepng.h"
#include "handycpp/image.h"
#include "lodepng.cpp"
namespace handycpp::image {
    rgba_data readPngAsRgba(std::string path) {
        rgba_data data;

        //decode
        unsigned error = lodepng::decode(data.rgba_image, data.width, data.height, path);

        //if there's an error, display it
        if(error) {
            FUN_ERROR("failed to decode %s, %s", path.c_str(), lodepng_error_text(error));
            data.width = 0;
            data.height = 0;
            return data;
        }
        return data;
    }
    /*
     * return 0 on success
     */
    int saveRgbaToPng(std::string path, unsigned char * rgba, int w, int h) {
        unsigned int error = lodepng::encode(path, rgba, w, h);
        return error;
    }

}
