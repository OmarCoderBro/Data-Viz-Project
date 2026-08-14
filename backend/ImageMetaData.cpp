#include "ImageMetaData.h"
#include <filesystem>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

ImageMetadata extractMetadata(const std::string& path) {
    fs::path filePath(path);

    ImageMetadata metadata;

    metadata.path = path;
    metadata.filename = filePath.filename().string();
    metadata.extension = filePath.extension().string();
    metadata.fileSize = fs::file_size(filePath);

    cv::Mat image = cv::imread(path);

    if (!image.empty()) {
        metadata.width = image.cols;
        metadata.height = image.rows;
    } else {
        metadata.width = 0;
        metadata.height = 0;
    }

    return metadata;
}