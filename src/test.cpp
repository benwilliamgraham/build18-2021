#include "oscopegl.hpp"
#include <iostream>
#include <math.h>
#include <unistd.h>

using namespace oscopegl;

int main() {
  Renderer renderer(96000);

  for (int i = 0;; i++) {
    renderer.buffer_polygon(
        {Point(-0.5, -0.5), Point(0.5, -0.5), Point(0, 0.5)}, DARK, LIGHT);
    renderer.buffer_polygon(
        {Point(0.1, 0.2), Point(1, -0.2), Point(1, 0.2)}, DARK, DARK);
    renderer.buffer_line(Point(-1, 0), Point(-0.1, 0.2), DARK);

    renderer.swap_buffer();

    sleep(100);
  }
}
