#include "oscopegl.hpp"
#include <iostream>
#include <math.h>
#include <unistd.h>

using namespace oscopegl;

#define SAMPLE_RATE 9600

int main() {
  Renderer *R = Renderer::instance();
  R->start(SAMPLE_RATE);

  for (int i = 0;; i++) {
    R->buffer_polygon(
        {Point(-0.5, -0.5), Point(0.5, -0.5), Point(0, 0.5)}, DARK, LIGHT);
    R->buffer_polygon(
        {Point(0.1, 0.2), Point(1, -0.2), Point(1, 0.2)}, DARK, DARK);
    R->buffer_line(Point(-1, 0), Point(-0.1, 0.2), DARK);

    R->swap_buffer();

    sleep(100);
  }
}
