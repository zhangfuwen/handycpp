//
// Created by zhangfuwen on 2022/3/1.
//

#include "handycpp/image.h"
#include "lodepng.cpp"
#include "lodepng.h"
namespace handycpp::image {
[[maybe_unused]] rgba_data readPngAsRgba(const std::string & path) {
    rgba_data data;

    // decode
    unsigned error = lodepng::decode(data.rgba_image, data.width, data.height, path);

    // if there's an error, display it
    if (error) {
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
[[maybe_unused]] int saveRgbaToPng(const std::string & path, const unsigned char *rgba, int w, int h) {
    unsigned int error = lodepng::encode(path, rgba, w, h);
    return (int)error;
}

} // namespace handycpp::image
