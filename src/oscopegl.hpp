#include <string>
#include <tuple>
#include <vector>

namespace oscopegl {
using Point = std::tuple<int, int>;

class Shape {
public:
  std::vector<Point> points;
  static Shape Line(Point a, Point b, unsigned fill);
  static Shape Triangle(Point a, Point b, Point c, unsigned border,
                        unsigned fill);
  static Shape Rectangle(Point location, Point size, unsigned border,
                         unsigned fill);
  static Shape Polygon(std::vector<Point> points, unsigned border,
                       unsigned fill);
  static Shape Text(std::string value, Point location, Point size,
                    unsigned fill);
};

class Renderer {
public:
  Renderer(unsigned width, unsigned height, unsigned sample_rate);
  void draw(std::vector<Shape> &shapes);
};
} // namespace oscopegl
