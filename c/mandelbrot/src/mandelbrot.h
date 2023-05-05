/*
 * =====================================================================================
 *
 *       Filename:  mandelbrot.h
 *
 *    Description:  mandelbrot header file
 *
 *        Version:  1.0
 *        Created:  05/03/2023 09:24:35 PM
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  Sunglow
 *
 * =====================================================================================
 */
#include <complex.h>
#include <stdint.h>
#ifndef MANDELBROT_H_
#define MANDELBROT_H_

// -- macros -------------------------------------------------------------------
#define return_defer(value)                                                    \
  do {                                                                         \
    result = (value);                                                          \
    goto defer;                                                                \
  } while (0)

// from: https://en.cppreference.com/w/c/numeric/complex/CMPLX
#define CMPLXL(x, y)                                                           \
  ((long double complex)((long double)(x) + I * (long double)(y)))

// -- types --------------------------------------------------------------------
struct Canvas {
  uint32_t width;
  uint32_t height;
  uint32_t pixels[];
};
typedef struct Canvas Canvas;

// -- functions ----------------------------------------------------------------

int save_to_ppm(Canvas *canvas, const char *file_path);
long double lerp(const long double a, const long double b, const long double t);
long double complex pixel_coordinate(Canvas canvas, const uint32_t index);
uint32_t eta(const long double complex coordinate,
             const uint32_t max_iteration);
void fill(Canvas *canvas, const uint32_t color);
void set_pixel(Canvas *canvas, const uint32_t index, const uint32_t color);
uint32_t color_construct(uint32_t iteration);

#endif // MANDELBROT_IMPLEMENTATION
