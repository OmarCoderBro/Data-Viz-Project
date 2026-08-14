#include "ImageScanner.h"
#include <filesystem>

namespace fs = std::filesystem;

std::vector<std::string> ImageScanner::scan(const std::string& directory) {
    std::vector<std::string> images;

    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (!entry.is_regular_file()) {
            continue;
        }

        std::string extension = entry.path().extension().string();

        if (extension == ".jpg" ||
            extension == ".jpeg" ||
            extension == ".png" ||
            extension == ".webp") {
            images.push_back(entry.path().string());
        }
    }

    return images;
}