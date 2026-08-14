#pragma once

#include <string>

struct ImageMetadata {
    std::string path;
    std::string filename;
    std::string extension;
    uintmax_t fileSize;
    int width;
    int height;
};

ImageMetadata extractMetadata(const std::string& path);