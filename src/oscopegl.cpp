#include "oscopegl.hpp"
#include <iostream>
#include <portaudio.h>

typedef struct {
  float left_phase;
  float right_phase;
} paData;

namespace oscopegl {

Shape Shape::Line(Point a, Point b, unsigned fill) {
  Shape shape;
  for (int i = 0; i < fill; i++) {
    shape.points.push_back(a);
    shape.points.push_back(b);
  }
  return shape;
}

Shape Shape::Rectangle(Point location, Point size, unsigned int border, unsigned int fill) {
  Shape shape;
  int x, y, w, h;
  std::tie(x, y) = location;
  std::tie(w, h) = size;
  for (int i = 0; i < border; i++) {
    shape.points.push_back(Point(x, y));
    shape.points.push_back(Point(x, y + h));
    shape.points.push_back(Point(x + w, y + h));
    shape.points.push_back(Point(x + w, y));
  }
  for (int i = 0; i < fill; i++) {
    for (;w > 8 && h > 8;) {
      x += 8;
      y += 8;
      w -= 16;
      h -= 16;
      shape.points.push_back(Point(x, y));
      shape.points.push_back(Point(x, y + h));
      shape.points.push_back(Point(x + w, y + h));
      shape.points.push_back(Point(x + w, y));
    }
  }
  return shape;
}

int screen_width, screen_height;

std::vector<Point> to_render;
std::vector<Point>::iterator it;

static int paCallback(const void *_in, void *void_out,
                      unsigned long frames_per_buffer,
                      const PaStreamCallbackTimeInfo *time_info,
                      PaStreamCallbackFlags _flags, void *void_data) {
  paData *data = (paData *)void_data;
  float *out = (float *)void_out;

  for (unsigned long i = 0; i < frames_per_buffer; i++) {
    int x, y;
    *out++ = data->left_phase;
    *out++ = data->right_phase;
    if (to_render.size() == 0) {
     continue; 
    }
    if (it == to_render.end()) {
      it = to_render.begin();
      }
    std::tie(x, y) = *it;
    if (rand() % 16 == 0) {
      ++it;
    }
    data->left_phase = 2.0 * (float)x / (float)screen_width - 1.0;
    data->right_phase = 2.0 * (float)y / (float)screen_height - 1.0;
  }
  return 0;
}

static paData data;
Renderer::Renderer(unsigned width, unsigned height, unsigned sample_rate) {
  screen_width = width;
  screen_height = height;

  PaStream *stream;

  data.left_phase = data.right_phase = 0.0;

  Pa_Initialize();
  Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, sample_rate, paFramesPerBufferUnspecified, paCallback,
                       &data);
  Pa_StartStream(stream);
}

void Renderer::draw(std::vector<Shape> &shapes) {
  to_render.clear();
  for (Shape shape : shapes) {
    to_render.insert(to_render.end(), shape.points.begin(), shape.points.end());
  }
  it = to_render.begin();
}

} // namespace oscopegl
