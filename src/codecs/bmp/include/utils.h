//
// Created by debilian on 10.01.2026.
//

#pragma once

#include <array>
#include <cstdint>
#include <cstring>
#include <string_view>

#include <bit>          // std::byteswap
#include <cstring>      // std::memcpy
#include <type_traits>  // std::is_enum_v, std::underlying_type_t
#include <cstddef>      // std::byte

template<typename T, std::endian ENDIANNESS = std::endian::little>
T read_numerical_value(std::byte *&cursor)
{
    T value{};

    std::memcpy(&value, cursor, sizeof(T));
    cursor += sizeof(T);

    if (ENDIANNESS != std::endian::native)
    {
        if constexpr (std::is_enum_v<T>) {
            using IntType = std::underlying_type_t<T>;
            value = static_cast<T>(std::byteswap(static_cast<IntType>(value)));
        }
        else {
            value = std::byteswap(value);
        }
    }

    return value;
}

template<typename T>
T read_array_value(std::byte *&cursor)
{
    T value{};
    std::memcpy(&value, cursor, sizeof(T));

    cursor += sizeof(T);

    return value;
}
