/*
 * =====================================================================================
 *
 *       Filename:  mandelbrot.c
 *
 *    Description:  mandelbrot helper functions
 *
 *        Version:  0.1
 *        Created:  05/03/2023 09:07:27 PM
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  Sunglow
 *
 * =====================================================================================
 */
#include "mandelbrot.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int save_to_ppm(Canvas *c, const char *file_path) {
  int result = 0;
  FILE *f = NULL;

  {
    f = fopen(file_path, "wb");
    if (f == NULL)
      return_defer(errno);

    fprintf(f, "P6\n%u %u 255\n", c->width, c->height);
    if (ferror(f))
      return_defer(errno);

    for (uint32_t i = 0; i < c->width * c->height; ++i) {
      const uint32_t pixel = c->pixels[i];
      const uint8_t bytes[3] = {(pixel >> (8 * 1)) & 0xFF,
                                (pixel >> (8 * 2)) & 0xFF,
                                (pixel >> (8 * 3)) & 0xFF};
      fwrite(bytes, sizeof(bytes), 1, f);
      if (ferror(f))
        return_defer(errno);
    }
  }

// error handling
defer:
  if (f)
    fclose(f);
  return result;
}

// from: https://en.wikipedia.org/wiki/Linear_interpolation
long double lerp(const long double a, const long double b,
                 const long double t) {
  return a * (1.0f - t) + (b * t);
}

long double complex pixel_coordinate(const Canvas c, const uint32_t index) {
  return CMPLXL(
      lerp(+1.235, -1.235, (index / c.height) / (long double)(c.height)),
      lerp(-2.000, +0.470, (index % c.width) / (long double)(c.width)));
}

// mandelbrot escape time algorithm
uint32_t eta(const long double complex pair, const uint32_t max_iteration) {
  long double x = 0;
  long double y = 0;
  long double x2 = 0;
  long double y2 = 0;
  uint32_t iteration = 0;

  while ((x2 + y2) <= 4 && iteration <= max_iteration) {
    y = (x + x) * y + creal(pair);
    x = x2 - y2 + cimag(pair);
    x2 = x * x;
    y2 = y * y;
    iteration++;
  }

  return iteration;
}

// mainly used for testing Canvas and other image functions
void fill(Canvas *c, const uint32_t color) {
  for (uint32_t i = 0; i < c->width * c->height; ++i)
    c->pixels[i] = color;
}

void set_pixel(Canvas *c, const uint32_t index, const uint32_t color) {
  c->pixels[index] = color;
}

uint32_t color_construct(uint32_t iteration) {
  const uint8_t colors[4] = {0xFF, iteration, iteration, iteration};
  return *(uint32_t *)colors;
}
