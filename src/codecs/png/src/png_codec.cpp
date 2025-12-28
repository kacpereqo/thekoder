////
//// Created by remza on 28.12.2025.
////
//
//#include "png_codec.hpp"
//
//
//if ( filestream.read(reinterpret_cast<char*>(image_data.data()), size)) {
//
//Logger::debug() << "Image data read from file: " << filename << std::endl;
//
//} else {
//
//Logger::debug() << "Failed to read image data from file: " << filename << std::endl;
//std::exit(EXIT_FAILURE);
//
//}
//
//if (is_png(image_data_cursor))
//Logger::debug() << "The file is a valid PNG image" << std::endl;
//else
//Logger::warning() << "The file is not a valid PNG image" << std::endl;
//
//Chunk chunk;
//IHDRData hdr_data{};
//GAMAData gama_data{};
//PHYSData phys_data{};
//IDATData idat_data{};
//
//while (chunk.chunk_type_str() != ChunkType::IEND) {
//chunk = read_chunk(image_data_cursor);
//
//const auto chunk_type = chunk.chunk_type_str();
//
//if (chunk_type == ChunkType::IHDR) hdr_data = IHDRData{std::move(chunk)};
//else if (chunk_type == ChunkType::SRGB) {}
//else if (chunk_type == ChunkType::GAMA) gama_data = GAMAData{std::move(chunk)};
//else if (chunk_type == ChunkType::PHYS) phys_data = PHYSData{std::move(chunk)};
//else if (chunk_type == ChunkType::IDAT) idat_data = IDATData{std::move(chunk)};
//else if (chunk_type == ChunkType::IEND) {}
//else                                    {}
//}
//
//uLongf vector_length = hdr_data.width * hdr_data.height * 3 + hdr_data.height;
//
//std::vector<uint8_t> pixels;
//pixels.reserve(vector_length);
//
//auto result = uncompress(pixels.data(),
//                         &vector_length,jakby
//reinterpret_cast<Bytef*>(idat_data.compressed_data.data()),
//idat_data.compressed_data.size());
//
//if (result == Z_OK) {
//Logger::debug() << "Uncompressed data read from file: " << filename << std::endl;
//} else {
//Logger::debug() << "Failed to uncompress data from file: " << filename << std::endl;
//}