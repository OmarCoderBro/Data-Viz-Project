#pragma once

#include <string>
#include <vector>

class ImageScanner {
public:
    std::vector<std::string> scan(const std::string& directory);
};