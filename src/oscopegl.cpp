#include "oscopegl.hpp"
#include <iostream>
#include <math.h>
#include <portaudio.h>

namespace oscopegl {

const float light_draw_len = 0.1;
const float dark_draw_len = light_draw_len / 4;

std::vector<float> buffer_points;
std::vector<float> draw_points;
std::vector<float>::iterator draw_itr;

float next_coord_() {
  if (draw_points.empty()) {
    return 0;
  }
  if (draw_itr == draw_points.end()) {
    draw_itr = draw_points.begin();
  }
  return *draw_itr++;
}

int callback_(const void *_in, void *void_out, unsigned long num_frames,
              const PaStreamCallbackTimeInfo *_time,
              PaStreamCallbackFlags _flags, void *_data) {
  float *out = (float *)void_out;
  for (unsigned long i = 0; i < num_frames; i++) {
    *out++ = next_coord_();
    *out++ = next_coord_();
  }
  return 0;
}

Renderer::Renderer(unsigned sample_rate) {
  PaStream *stream;

  Pa_Initialize();
  Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, sample_rate,
                       paFramesPerBufferUnspecified, callback_, NULL);
  Pa_StartStream(stream);
}

void Renderer::buffer_line(Point a, Point b, Color fill) {
  float draw_len = fill == LIGHT ? light_draw_len : dark_draw_len;
  float x = a.x, y = a.y;
  while (true) {
    buffer_points.push_back(x);
    buffer_points.push_back(y);
    float x_dist = b.x - x, y_dist = b.y - y;
    float dist = sqrt(x_dist * x_dist + y_dist * y_dist);
    if (dist < draw_len) {
      break;
    }
    x += draw_len * x_dist / dist;
    y += draw_len * y_dist / dist;
  }
  buffer_points.push_back(b.x);
  buffer_points.push_back(b.y);
}

void Renderer::buffer_triangle(Point a, Point b, Point c, Color border,
                               Color fill) {}

void Renderer::buffer_rectangle(Point location, Point size, Color border,
                                Color fill) {
  float x = location.x, y = location.y, w = size.x, h = size.y;
  buffer_line(Point(x, y),
              Point(x + w, y), border);
  buffer_line(Point(x + w, y),
              Point(x + w, y + h), border);
  buffer_line(Point(x + w, y + h),
              Point(x, y + h), border);
  buffer_line(Point(x, y + h),
              Point(x, y), border);
  while (w > 0.04 && h > 0.04) {
      x += 0.02;
      y += 0.02;
      w -= 0.04;
      h -= 0.04;
  buffer_line(Point(x, y),
              Point(x + w, y), fill);
  buffer_line(Point(x + w, y),
              Point(x + w, y + h), fill);
  buffer_line(Point(x + w, y + h),
              Point(x, y + h), fill);
  buffer_line(Point(x, y + h),
              Point(x, y), fill);
  }
}

void Renderer::buffer_polygon(std::vector<Point> points, Color border,
                              Color fill) {}

void Renderer::buffer_text(std::string value, Point location, Point size,
                           Color fill) {}

void Renderer::swap_buffer() {
  draw_points = buffer_points;
  draw_itr = draw_points.begin();
  buffer_points.clear();
}

} // namespace oscopegl
