//
// Created by debilian on 10.01.2026.
//

#pragma once

#include <bit>
#include <cstddef>
#include <cstring>
#include <type_traits>

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