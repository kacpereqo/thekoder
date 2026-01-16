//
// Created by remza on 11.01.2026.
//

#pragma once

#include "chunks.hpp"
#include "constants.hpp"

class GIF
{
public:
    Signature signature;
    LogicalScreenDescriptor logical_screen_descriptor;
    GlobalColorTable global_color_table;
};
