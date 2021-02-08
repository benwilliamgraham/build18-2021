#include "oscopegl.hpp"
#include <iostream>
#include <math.h>
#include <portaudio.h>

namespace oscopegl {

const float light_draw_len = 0.08;
const float dark_draw_len = light_draw_len / 2;

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

void buffer_point(Point p, const Color fill) {
  for (int i = 0; i < (fill == LIGHT ? 2 : 4); i++) {
    buffer_points.push_back(p.x);
    buffer_points.push_back(p.y);
  }
}

void Renderer::buffer_line(Point a, Point b, const Color fill) {
  float draw_len = fill == LIGHT ? light_draw_len : dark_draw_len;
  float x = a.x, y = a.y;
  while (true) {
    buffer_points.push_back(x);
    buffer_points.push_back(y);
    float x_dist = b.x - x, y_dist = b.y - y;
    float dist = sqrt(x_dist * x_dist + y_dist * y_dist);
    if (dist <= draw_len) {
      break;
    }
    x += draw_len * x_dist / dist;
    y += draw_len * y_dist / dist;
  }
  buffer_points.push_back(b.x);
  buffer_points.push_back(b.y);
}

void Renderer::buffer_polygon(std::vector<Point> points, const Color border,
                              const Color fill) {
  float draw_len = fill == LIGHT ? light_draw_len : dark_draw_len;

  // draw outline and calculate center
  Point center(0, 0);
  for (int i = 0; i < points.size(); i++) {
    buffer_line(points[i], points[(i + 1) % points.size()], border);
    center.x += points[i].x;
    center.y += points[i].y;
  }
  center.x /= points.size();
  center.y /= points.size();

  // spiral inwards until center is reached
  while (fill != NONE) {
    // shift points towards center
    for (int i = 0; i < points.size(); i++) {
      Point p = points[i];
      float x_dist = center.x - p.x, y_dist = center.y - p.y;
      float dist = sqrt(x_dist * x_dist + y_dist * y_dist);
      if (dist <= draw_len) {
        goto done;
      }
      p.x += draw_len * x_dist / dist;
      p.y += draw_len * y_dist / dist;
      points[i] = p;
    }

    // draw spiral
    for (int i = 0; i < points.size(); i++) {
      buffer_line(points[i], points[(i + 1) % points.size()], fill);
    }
  }
done:;
}

void Renderer::buffer_rectangle(Point location, Point size, const Color border,
                                const Color fill) {
  buffer_polygon({Point(location.x, location.y),
                  Point(location.x + size.x, location.y),
                  Point(location.x + size.x, location.y + size.y),
                  Point(location.x, location.y + size.y)},
                 border, fill);
}

void Renderer::buffer_text(std::string value, Point location, Point size,
                           const Color fill) {}

void Renderer::swap_buffer() {
  draw_points = buffer_points;
  draw_itr = draw_points.begin();
  buffer_points.clear();
}

} // namespace oscopegl
