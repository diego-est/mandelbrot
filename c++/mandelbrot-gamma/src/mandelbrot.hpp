#pragma once
#include <cstdint>
using size_t = std::size_t;
#include <array>
#include <complex>
#include <fstream>
#include <iostream>

struct RGB {
	uint8_t R;
	uint8_t G;
	uint8_t B;

	RGB();
	RGB(uint8_t r, uint8_t g, uint8_t b);
};

struct Color {
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
	uint8_t Alpha;

	Color();
	Color(uint32_t val);

	auto to_rgb() -> RGB;
};

template <size_t width, size_t height> class Canvas {
      public:
	size_t m_width = width;
	size_t m_height = height;
	std::array<Color, width * height> pixels;

	auto save_to_ppm(const char* file_path) const -> void
	{
		auto f =
		    std::fstream(file_path, std::ios::binary | std::ios::out);
		f << "P6\n" << width << " " << height << " 255\n";
		for (auto const& color : pixels) {
			uint8_t rgb[3] = {color.Red, color.Green, color.Blue};
			f << rgb;
		}
		f.close();
	}

	auto coordinate(size_t index) const -> std::complex<long double>
	{
		return std::complex<long double>(
		    std::lerp(-2.000, 0.470,
			      (index % width) / (long double)width),
		    std::lerp(1.235, -1.235,
			      ((long double)index / height) / height));
	}

	auto construct(auto op) -> void
	{
#pragma omp parallel for
		for (size_t idx = 0; idx < width * height; idx++)
			pixels[idx] = op(coordinate(idx));
	}

	auto operator[](size_t index) -> Color&
	{
		return pixels[index];
	}

	auto operator()(size_t row, size_t col) -> Color&
	{
		return pixels[row * width + col];
	}
};
