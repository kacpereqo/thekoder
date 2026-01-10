//
// Created by remza on 28.12.2025.
//

#pragma once

#include <vector>

class Codec
{

public:
    using data_cursor = std::byte *&;

    virtual auto decode(std::byte *&cursor) -> std::vector<std::byte>;

    virtual auto is_valid() -> bool;
    virtual auto is_signature_valid() -> bool;

    virtual ~Codec() = default;
};
