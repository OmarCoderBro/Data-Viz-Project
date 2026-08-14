#include <iostream>
#include "ImageMetaData.h"
#include "ImageScanner.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cout << "Usage: ./image_intelligence <directory>" << std::endl;
        return 1;
    }

    ImageScanner scanner;
    std::vector<std::string> images = scanner.scan(argv[1]);

    std::cout << "Found " << images.size() << " images:\n\n";

    for (const auto& image : images) {
        ImageMetadata metadata = extractMetadata(image);

        std::cout << "File: " << metadata.filename << std::endl;
        std::cout << "Extension: " << metadata.extension << std::endl;
        std::cout << "Size: " << metadata.fileSize << " bytes" << std::endl;
        std::cout << "------------------------" << std::endl;
        std::cout << "Dimensions: " << metadata.width
          << " x " << metadata.height << std::endl;
    }

    return 0;
}