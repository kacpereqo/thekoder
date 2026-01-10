#pragma once

#include <cstdint>
#include <iostream>

struct RGB8;
struct RGBA8;
struct RGB16;
struct RGBA16;

struct RGB8
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;

    RGB8() = default;
    RGB8(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b)
    {}

    explicit RGB8(const RGBA8 &c);
    explicit RGB8(const RGB16 &c);
    explicit RGB8(const RGBA16 &c);

    friend std::ostream &operator<<(std::ostream &os, const RGB8 &c)
    {
        return os << "RGB8(" << static_cast<int>(c.r) << ", " << static_cast<int>(c.g) << ", " << static_cast<int>(c.b)
                  << ")";
    }
};

struct RGBA8
{
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;

    RGBA8() = default;
    RGBA8(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a)
    {}

    explicit RGBA8(const RGB8 &c);
    explicit RGBA8(const RGB16 &c);
    explicit RGBA8(const RGBA16 &c);

    friend std::ostream &operator<<(std::ostream &os, const RGBA8 &c)
    {
        return os << "RGBA8(" << static_cast<int>(c.r) << ", " << static_cast<int>(c.g) << ", " << static_cast<int>(c.b)
                  << ", " << static_cast<int>(c.a) << ")";
    }
};

struct RGB16
{
    uint16_t r = 0;
    uint16_t g = 0;
    uint16_t b = 0;

    RGB16() = default;
    RGB16(uint16_t r, uint16_t g, uint16_t b) : r(r), g(g), b(b)
    {}

    RGB16(const RGB8 &c);
    RGB16(const RGBA8 &c);
    RGB16(const RGBA16 &c);

    friend std::ostream &operator<<(std::ostream &os, const RGB16 &c)
    {
        return os << "RGB16(" << c.r << ", " << c.g << ", " << c.b << ")";
    }
};

struct RGBA16
{
    uint16_t r = 0;
    uint16_t g = 0;
    uint16_t b = 0;
    uint16_t a = 65535;

    RGBA16() = default;
    RGBA16(uint16_t r, uint16_t g, uint16_t b, uint16_t a) : r(r), g(g), b(b), a(a)
    {}

    RGBA16(const RGB8 &c);
    RGBA16(const RGBA8 &c);
    RGBA16(const RGB16 &c);

    friend std::ostream &operator<<(std::ostream &os, const RGBA16 &c)
    {
        return os << "RGBA16(" << c.r << ", " << c.g << ", " << c.b << ", " << c.a << ")";
    }
};

inline RGB8::RGB8(const RGBA8 &c) : r(c.r), g(c.g), b(c.b)
{} // Drop Alpha
inline RGB8::RGB8(const RGB16 &c) : r(c.r >> 8), g(c.g >> 8), b(c.b >> 8)
{} // Downscale
inline RGB8::RGB8(const RGBA16 &c) : r(c.r >> 8), g(c.g >> 8), b(c.b >> 8)
{} // Downscale + Drop Alpha

inline RGBA8::RGBA8(const RGB8 &c) : r(c.r), g(c.g), b(c.b), a(255)
{} // Add Opaque Alpha
inline RGBA8::RGBA8(const RGB16 &c) : r(c.r >> 8), g(c.g >> 8), b(c.b >> 8), a(255)
{} // Downscale + Add Alpha
inline RGBA8::RGBA8(const RGBA16 &c) : r(c.r >> 8), g(c.g >> 8), b(c.b >> 8), a(c.a >> 8)
{} // Downscale

inline RGB16::RGB16(const RGB8 &c) : r((c.r << 8) | c.r), g((c.g << 8) | c.g), b((c.b << 8) | c.b)
{}
inline RGB16::RGB16(const RGBA8 &c) : r((c.r << 8) | c.r), g((c.g << 8) | c.g), b((c.b << 8) | c.b)
{}
inline RGB16::RGB16(const RGBA16 &c) : r(c.r), g(c.g), b(c.b)
{} // Drop Alpha

inline RGBA16::RGBA16(const RGB8 &c) : r((c.r << 8) | c.r), g((c.g << 8) | c.g), b((c.b << 8) | c.b), a(65535)
{}
inline RGBA16::RGBA16(const RGBA8 &c) :
    r((c.r << 8) | c.r), g((c.g << 8) | c.g), b((c.b << 8) | c.b), a((c.a << 8) | c.a)
{}
inline RGBA16::RGBA16(const RGB16 &c) : r(c.r), g(c.g), b(c.b), a(65535)
{}
