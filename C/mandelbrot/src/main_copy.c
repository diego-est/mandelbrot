/* ========================================================================
 *
 *     Filename:  main.c
 *  Description:  mandelbrot set renderer written in C
 *      Version:  0.0.1
 *
 * ======================================================================== */
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define WIDTH 1023
#define HEIGHT WIDTH
#define MAXITER 1000

// declare canvas struct and typedef so it can be passed to functions
struct Canvas {
  uint32_t pixels[HEIGHT * WIDTH];
  uint32_t width;
  uint32_t height;
};
typedef struct Canvas Canvas;

// function prototypes
int save_to_ppm(Canvas canvas, const char *file_path);
void fill(Canvas *canvas, uint32_t color);
void set_pixel(Canvas *canvas, uint32_t i, uint32_t color);
float lerp(long double, long double, long double);

int main(void) {
  // printf("%zu\n", sizeof(uint32_t));
  struct Canvas canvas;
  canvas.width = WIDTH;
  canvas.height = HEIGHT;

  for (int i = 0; i < WIDTH * HEIGHT; ++i) {
    // lerp between these values for the reals and imaginaries
    // -2.00 <-> 0.47 = 2.47
    // -1.12 <-> 1.12 = 2.24
    float real = lerp(-2.00, .47, (i % WIDTH) / (long double)(WIDTH));
    float imag = lerp(1.235, -1.235, (i / HEIGHT) / (long double)(HEIGHT));

    long double x = 0.0;
    long double y = 0.0;
    long double xx = 0.0;
    long double yy = 0.0;
    uint32_t iteration = 0;

    while ((xx + yy) <= 4 && iteration <= MAXITER) {
      y = (x + x) * y + imag;
      x = xx - yy + real;
      xx = x * x;
      yy = y * y;
      iteration++;
    }
    uint32_t b = lerp(0, 255, iteration / (long double)MAXITER);
    //                  pixels 0xAA  BB GG RR
    const uint8_t colors[4] = {0xFF, b, b, b};
    const uint32_t color = *(uint32_t *)colors;

    set_pixel(&canvas, i, color);
  }
  save_to_ppm(canvas, "output.ppm");
  return 0;
}

void fill(Canvas *canvas, uint32_t color) {
  for (uint32_t i = 0; i < canvas->width * canvas->height; ++i)
    canvas->pixels[i] = color;
}

// useful for error handling in save_to_ppm()
#define return_defer(value)                                                    \
  do {                                                                         \
    result = (value);                                                          \
    goto defer;                                                                \
  } while (0)
int save_to_ppm(Canvas canvas, const char *file_path) {
  int result = 0;
  FILE *f = NULL;

  {
    f = fopen(file_path, "wb");
    if (f == NULL)
      return_defer(errno);

    fprintf(f, "P6\n%u %u 255\n", canvas.width, canvas.height);
    if (ferror(f))
      return_defer(errno);

    for (uint32_t i = 0; i < canvas.width * canvas.height; ++i) {
      const uint32_t pixel = canvas.pixels[i];
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

void set_pixel(Canvas *canvas, uint32_t i, uint32_t color) {
  canvas->pixels[i] = color;
}

// different from the "naive" lerp for extra precision
float lerp(long double a, long double b, long double t) {
  return a * (1.0 - t) + (b * t);
}
