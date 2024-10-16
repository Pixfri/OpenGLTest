// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of OpenGL Test.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include <cstdint>

namespace OGLTest {
    using UInt8 = uint8_t;
    using UInt16 = uint16_t;
    using UInt32 = uint32_t;
    using UInt64 = uint64_t;
    
    using Int8 = int8_t;
    using Int16 = int16_t;
    using Int32 = int32_t;
    using Int64 = int64_t;

    using Float32 = float;
    using Float64 = double;

    static_assert(sizeof(UInt8) == 1, "UInt8 is supposed to be 8 bits long.");
    static_assert(sizeof(UInt16) == 2, "UInt16 is supposed to be 16 bits long.");
    static_assert(sizeof(UInt32) == 4, "UInt32 is supposed to be 32 bits long.");
    static_assert(sizeof(UInt64) == 8, "UInt64 is supposed to be 64 bits long.");

    static_assert(sizeof(Int8) == 1, "Int8 is supposed to be 8 bits long.");
    static_assert(sizeof(Int16) == 2, "Int16 is supposed to be 16 bits long.");
    static_assert(sizeof(Int32) == 4, "Int32 is supposed to be 32 bits long.");
    static_assert(sizeof(Int64) == 8, "Int64 is supposed to be 64 bits long.");
    
    static_assert(sizeof(Float32) == 4, "Float32 is supposed to be 32 bits long.");
    static_assert(sizeof(Float64) == 8, "Float64 is supposed to be 64 bits long.");
}
