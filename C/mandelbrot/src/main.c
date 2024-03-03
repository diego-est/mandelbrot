/* ========================================================================
 *
 *     Filename:  main.c
 *  Description:  mandelbrot set renderer written in C
 *      Version:  0.0.1
 *
 * ======================================================================== */
#include "mandelbrot.h"
#include <stdlib.h>
#include <string.h>

#define WIDTH 800
#define HEIGHT WIDTH
#define MAXITER 254

int main(void) {
  uint32_t max_iteration = MAXITER;

  Canvas *canvas = malloc(sizeof(Canvas) + WIDTH * HEIGHT * sizeof(uint32_t));
  canvas->width = WIDTH;
  canvas->height = HEIGHT;

  for (uint32_t index = 0; index < WIDTH * HEIGHT; ++index) {
    const long double complex pair = pixel_coordinate(*canvas, index);
    const uint32_t iteration = eta(pair, max_iteration);
    const uint32_t color = color_construct(iteration);
    set_pixel(canvas, index, color);

    // clang-format off
    // set_pixel(canvas, index, color_construct(eta(pixel_coordinate(*canvas, index), max_iteration)));
    // clang-format on
  }

  save_to_ppm(canvas, "output.ppm");

  return 0;
}
