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
  void buffer_point(Point p, const Color fill);
  void buffer_line(Point a, Point b, const Color fill);
  void buffer_polygon(std::vector<Point> points, const Color border, const Color fill);
  void buffer_rectangle(Point location, Point size, const Color border, const Color fill);
  void buffer_text(std::string value, Point location, Point size, const Color fill);
  void swap_buffer();
};

} // namespace oscopegl
