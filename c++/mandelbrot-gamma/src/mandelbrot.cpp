#include "mandelbrot.hpp"
#include <cmath>
#include <fstream>
#include <ranges>
#include <utility>

RGB::RGB() : R(0), G(0), B(0)
{
}

RGB::RGB(uint8_t r, uint8_t g, uint8_t b) : R(r), G(g), B(b)
{
}

Color::Color() : Red(0), Green(0), Blue(0), Alpha(0)
{
}

Color::Color(uint32_t val)
{
	Red = Green = Blue = val & 0xFF;
	Alpha = 0xFF;
}

auto Color::to_rgb() -> RGB
{
	return RGB(Red, Green, Blue);
}
