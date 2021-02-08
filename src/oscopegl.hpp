#include <string>
#include <tuple>
#include <vector>

#pragma once
#include <portaudio.h>

namespace oscopegl {

enum Color { NONE, LIGHT, DARK };


class Point {
public:
  float x, y;
  Point(float x, float y) : x(x), y(y) {}
};

float next_coord_();
int callback_(const void *_in, void *void_out, unsigned long num_frames, const PaStreamCallbackTimeInfo *_time, PaStreamCallbackFlags _flags, void *data);

class Renderer {
public:
  // Get the instance of the renderer
  static Renderer *instance();

  // Constructor
  Renderer(unsigned sample_rate);
  // Add a point to the buffer
  void buffer_point(Point p, const Color fill, bool persist = false);
  // Add a line to the buffer
  void buffer_line(Point a, Point b, const Color fill, bool persist = false);
  // Add a polygon to the buffer
  void buffer_polygon(std::vector<Point> points, const Color border, const Color fill, bool persist = false);
  // Add a rectangle to the buffer
  void buffer_rectangle(Point location, Point size, const Color border, const Color fill, bool persist = false);
  // Add text to the buffer
  void buffer_text(std::string value, Point location, Point size, const Color fill, bool persist = false);

  // Swap buffer with currently drawn buffer
  void swap_buffer();
  // Clear drawn and persistent buffer
  void clear();
  // Get the next coordinate
  float next_coord_();

private:

  const float light_draw_len = 0.08;
  const float dark_draw_len = light_draw_len / 2;
  std::vector<float> buffer_points;
  std::vector<float> draw_points;
  std::vector<float> persist_draw_points;
  std::vector<float>::iterator draw_itr;
};

} // namespace oscopegl
