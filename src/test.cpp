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
        Shape::Line(Point(200 + sin(angle) * 100, 200 + cos(angle) * 100),
                    Point(200 - sin(angle) * 100, 200 - cos(angle) * 100), 8),
        Shape::Line(Point(200 + sin(angle + 0.5) * 100, 200 + cos(angle + 0.5) * 100),
                    Point(200 - sin(angle + 0.5) * 100, 200 - cos(angle + 0.5) * 100), 8)};
    angle += 0.0001;

    renderer.draw(shapes);

    usleep(10);
  }
}
