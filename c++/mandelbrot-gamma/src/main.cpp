#include "mandelbrot.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <vector>

int main()
{
	static auto canvas = Canvas<2000, 2000>();
	// static constexpr auto histogram[] = [](){ return { 0, 1 }; }();
	auto mandelbrot = [](std::complex<long double> coordinate) {
		const auto max_iteration = 20000ul;
		auto x = 0.0L;
		auto y = 0.0L;
		auto x_squared = 0.0L;
		auto y_squared = 0.0L;
		auto iteration = 0u;

		while (std::islessequal((x_squared + y_squared), 4) &&
		       (iteration < max_iteration)) {
			y = fma(2 * x, y, coordinate.imag());
			x = x_squared - y_squared + coordinate.real();
			x_squared = x * x;
			y_squared = y * y;
			iteration++;
		}
		return Color(iteration);
	};

	canvas.construct(mandelbrot);
	canvas.save_to_ppm("image.ppm");

	return 0;
}
