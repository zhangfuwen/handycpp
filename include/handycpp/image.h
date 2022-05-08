//
// Created by zhangfuwen on 2022/2/21.
//

#ifndef HANDYCPP_IMAGE_H
#define HANDYCPP_IMAGE_H

#include "handycpp/file.h"
#include <cstring>

//#define STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_WRITE_IMPLEMENTATION

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#ifdef STB_IMAGE_IMPLEMENTATION
#include "handycpp/stb_image.h"
#endif

#ifdef STB_IMAGE_WRITE_IMPLEMENTATION
#include "handycpp/stb_image_write.h"
#endif

#pragma GCC diagnostic pop

namespace handycpp::image {

struct rgba_data {
    unsigned width;
    unsigned height;
    unsigned channels;
    std::vector<unsigned char> rgba_image;
};

#ifdef STB_IMAGE_IMPLEMENTATION
[[maybe_unused]] inline rgba_data readPngAsRgba(const std::string &path) {
    rgba_data data;
    auto ret = stbi_load(path.c_str(), reinterpret_cast<int *>(&data.width), reinterpret_cast<int *>(&data.height),
        reinterpret_cast<int *>(&data.channels), 0);
    data.rgba_image.resize(data.width * data.height * data.channels);
    memcpy(data.rgba_image.data(), ret, data.width * data.height * data.channels);
    stbi_image_free(ret);
    return data;
}
#else

[[maybe_unused]] rgba_data readPngAsRgba(const std::string &path);
#endif

#ifdef STB_IMAGE_WRITE_IMPLEMENTATION
[[maybe_unused]] int saveRgbaToPng(const std::string &outPath, const unsigned char *rgba, int w, int h) {
    return stbi_write_png(outPath.c_str(), w, h, 4, rgba, 4*w);
}
#else
[[maybe_unused]] int saveRgbaToPng(const std::string &outPath, const unsigned char *rgba, int w, int h);
#endif

[[maybe_unused]] static inline bool
writeBmp(const std::string &outPath, const unsigned char *rgb, int w, int h, int pixel_stride = 3) {
    FILE *f;
    int filesize = 54 + 3 * w * h; // w is your image width, h is image height, both int

    auto img = (unsigned char *)malloc(3 * w * h);
    if (img == nullptr) {
        return false;
    }
    memset(img, 0, 3 * w * h);
    unsigned char r, g, b;

    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            int x = i;
            int y = (h - 1) - j;
            r = *(rgb + w * y * pixel_stride);
            g = *(rgb + w * y * pixel_stride + 1);
            b = *(rgb + w * y * pixel_stride + 2);
            img[(x + y * w) * 3 + 2] = (unsigned char)(r);
            img[(x + y * w) * 3 + 1] = (unsigned char)(g);
            img[(x + y * w) * 3 + 0] = (unsigned char)(b);
        }
    }

    unsigned char bmpfileheader[14] = {'B', 'M', 0, 0, 0, 0, 0, 0, 0, 0, 54, 0, 0, 0};
    unsigned char bmpinfoheader[40] = {40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0};
    unsigned char bmppad[3] = {0, 0, 0};

    bmpfileheader[2] = (unsigned char)(filesize);
    bmpfileheader[3] = (unsigned char)(filesize >> 8);
    bmpfileheader[4] = (unsigned char)(filesize >> 16);
    bmpfileheader[5] = (unsigned char)(filesize >> 24);

    bmpinfoheader[4] = (unsigned char)(w);
    bmpinfoheader[5] = (unsigned char)(w >> 8);
    bmpinfoheader[6] = (unsigned char)(w >> 16);
    bmpinfoheader[7] = (unsigned char)(w >> 24);
    bmpinfoheader[8] = (unsigned char)(h);
    bmpinfoheader[9] = (unsigned char)(h >> 8);
    bmpinfoheader[10] = (unsigned char)(h >> 16);
    bmpinfoheader[11] = (unsigned char)(h >> 24);

    if (file::is_file_exist(outPath.c_str())) {
        file::removeFile(outPath);
    }
    f = fopen(outPath.c_str(), "w+");
    if (f == nullptr) {
        auto err = errno;
        free(img);
        return false;
    }
    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);
    for (int i = 0; i < h; i++) {
        fwrite(img + (w * (h - i - 1) * 3), 3, w, f);
        fwrite(bmppad, 1, (4 - (w * 3) % 4) % 4, f);
    }

    free(img);
    fclose(f);
    return true;
}
} // namespace handycpp::image

#endif // HANDYCPP_IMAGE_H
