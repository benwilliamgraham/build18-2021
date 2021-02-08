#include "oscopegl.hpp"
#include <iostream>
#include <math.h>
#include <unistd.h>

using namespace oscopegl;

int main() {
  Renderer renderer(96000);

  for (int i = 0;; i++) {
    renderer.buffer_rectangle(Point(-0.5, -0.5), Point(1, 1), DARK, DARK);

    renderer.swap_buffer();

    sleep(100);
  }
}
