#include <cstddef>
#include <fstream>
#include <iostream>
#include <vector>

#include "logger.hpp"
#include "png_codec.hpp"

auto read_file_data(const std::string_view filename)
{

    std::ifstream filestream(filename.data(), std::ios::binary | std::ios::ate);

    if (!filestream)
    {

        Logger::debug() << "Failed to open file " << filename;
        std::exit(EXIT_FAILURE);
    }
    Logger::info() << "Successfully opened file " << filename;

    const std::streamsize size = filestream.tellg();

    filestream.seekg(0, std::ios::beg);

    std::vector<std::byte> image_data(size);

    if (filestream.read(reinterpret_cast<char *>(image_data.data()), size))
    {
        Logger::debug() << "Image data read from file: " << filename << std::endl;
    } else
    {
        Logger::debug() << "Failed to read image data from file: " << filename << std::endl;
        std::exit(EXIT_FAILURE);
    }

    filestream.close();


    return image_data;
}

int main()
{
    constexpr std::string_view filename = "../examples/sample1.png";

    auto image_data = read_file_data(filename);
    auto data_cursor = image_data.data();

    PngCodec png(data_cursor);
    for (auto pixels = png.decode_to_rgb8(); auto x : pixels)
    {
        std::cout << x << std::endl;
    }

}