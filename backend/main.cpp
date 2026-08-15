#include <iostream>
#include "ImageMetaData.h"
#include "ImageScanner.h"
#include "ImageClassifier.h"
#include <sw/redis++/redis++.h>

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Usage: ./image_intelligence <directory>" << std::endl;
        return 1;
    }

    // Connect to local Redis
    sw::redis::Redis redis("tcp://127.0.0.1:6379");

    ImageScanner scanner;
    std::vector<std::string> images = scanner.scan(argv[1]);

    std::cout << "Found " << images.size() << " images:\n\n";

    for (const auto& image : images) {
        ImageMetadata metadata = extractMetadata(image);

        ImageClassifier classifier;
        std::string orientation = classifier.classifyOrientation(metadata);

        std::cout << "Orientation: " << orientation << std::endl;
        std::cout << "File: " << metadata.filename << std::endl;
        std::cout << "Extension: " << metadata.extension << std::endl;
        std::cout << "Size: " << metadata.fileSize << " bytes" << std::endl;
        std::cout << "Dimensions: " << metadata.width
                  << " x " << metadata.height << std::endl;
        std::cout << "Aspect Ratio: " << metadata.aspectRatio << std::endl;

        
        // Send image path to Python worker through Redis
        redis.lpush("image_jobs", image);

        std::cout << "Job submitted to Redis." << std::endl;
        std::cout << "------------------------" << std::endl;
    }

    return 0;
}