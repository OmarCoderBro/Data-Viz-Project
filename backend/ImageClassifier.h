#pragma once

#include "ImageMetaData.h"
#include <string>

class ImageClassifier {
public:
    std::string classifyOrientation(const ImageMetadata& metadata);
};