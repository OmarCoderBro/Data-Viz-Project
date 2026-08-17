cmake_minimum_required(VERSION 3.20)

project(ImageFileIntelligence)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)
find_package(redis++ CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)

add_executable(image_intelligence
    backend/main.cpp
    backend/ImageScanner.cpp
    backend/ImageMetadata.cpp
    backend/ImageClassifier.cpp
)

target_link_libraries(image_intelligence PRIVATE
    ${OpenCV_LIBS}
    redis++::redis++_static
    Crow::Crow
)

target_include_directories(image_intelligence PRIVATE
    ${OpenCV_INCLUDE_DIRS}
)