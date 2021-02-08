#include "oscopegl.hpp"
#include <iostream>
#include <math.h>
#include <unistd.h>

using namespace oscopegl;

int main() {
  Renderer *R = Renderer::instance();

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
