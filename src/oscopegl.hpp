#include <string>
#include <tuple>
#include <vector>

namespace oscopegl {

enum Color { NONE, LIGHT, DARK };

class Point {
public:
  float x, y;
  Point(float x, float y) : x(x), y(y) {}
};

class Renderer {
public:
  Renderer(unsigned sample_rate);
  void buffer_point(Point p, Color fill);
  void buffer_line(Point a, Point b, Color fill);
  void buffer_polygon(std::vector<Point> points, Color border, Color fill);
  void buffer_rectangle(Point location, Point size, Color border, Color fill);
  void buffer_text(std::string value, Point location, Point size, Color fill);
  void swap_buffer();
};

} // namespace oscopegl
