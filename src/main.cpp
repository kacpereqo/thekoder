#include <iostream>
#include <fstream>
#include <vector>
#include <cstddef>
#include <cstring>

#include "png_codec.hpp"
#include "logger/include/logger.hpp"

int main() {
    constexpr std::string_view filename = "../examples/sample1.png";

    std::ifstream filestream(filename.data(), std::ios::binary | std::ios::ate);

    if (!filestream) {

        Logger::debug() << "Failed to open file " << filename;
        std::exit(EXIT_FAILURE);

    }
    Logger::info() << "Successfully opened file " << filename;

    const std::streamsize size = filestream.tellg();

    filestream.seekg(0, std::ios::beg);

    std::vector<std::byte> image_data(size);

    if ( filestream.read(reinterpret_cast<char*>(image_data.data()), size)) {
    Logger::debug() << "Image data read from file: " << filename << std::endl;
    } else {
    Logger::debug() << "Failed to read image data from file: " << filename << std::endl;
    std::exit(EXIT_FAILURE);
    }

    filestream.close();

    auto image_data_cursor = image_data.data();

    PngCodec png_codec;

    PngCodec::is_signature_valid(image_data_cursor);
    auto pixels = png_codec.decode_to_rgb(image_data_cursor);

    for (int x = 0 ; x < png_codec.get_ihdr().width ; x++) {
        for (int y = 0 ; y < png_codec.get_ihdr().height; y++) {
            std::cout << pixels[y * png_codec.get_ihdr().width + x] << " ";
        }
        std::cout << std::endl;
    }

}
