#include "oscopegl.hpp"
#include <iostream>
#include <math.h>
#include <unistd.h>

using namespace oscopegl;

int main() {
  Renderer renderer(400, 400, 44100);

  float angle = 0;
  for (int i = 0;;i++) {
    std::vector<Shape> shapes{
      Shape::Rectangle(Point(100, 100), Point(200, 200), 8, 8)
      // Shape::Line(Point(0, 0), Point(200, 200), 8),
        // Shape::Line(Point(30, 30), Point(150, 150), 8)
    };
    angle += 0.0001;

    renderer.draw(shapes);

    sleep(1000);
  }
}
