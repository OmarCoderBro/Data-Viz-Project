#include "ImageClassifier.h"

std::string ImageClassifier::classifyOrientation(
    const ImageMetadata& metadata) {

    if (metadata.width == metadata.height) {
        return "Square";
    }

    if (metadata.width > metadata.height) {
        return "Landscape";
    }

    return "Portrait";
}