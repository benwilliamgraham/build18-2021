#include "oscopegl.hpp"
#include <iostream>
#include <math.h>
#include <unistd.h>

using namespace oscopegl;

class Point3d {
public:
  float x, y, z;
  Point3d(float x, float y, float z) : x(x), y(y), z(z) {}
};

class Face {
public:
  Point3d a, b, c;
  Face(Point3d a, Point3d b, Point3d c) : a(a), b(b), c(c) {}
};

void transform_(Point3d *v, float ys, float yc, float ps, float pc) {
  *v = Point3d(v->x * yc - v->z * ys, -v->y, v->z * yc + v->x * ys);
  *v = Point3d(v->x, v->y * pc - v->z * ps, v->z * pc + v->y * ps);
}

int main() {
  Renderer renderer(96000);

  FILE *fp = fopen("low-poly-teapot.obj", "r");
  std::vector<Point3d> vertices;
  Point3d vertex(0, 0, 0);
  float max = 0;
  // load vertices
  while (fscanf(fp, "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z) == 3) {
    vertices.push_back(vertex);
    max = std::max(std::max(std::max(std::abs(vertex.x), std::abs(vertex.y)),
                            std::abs(vertex.z)),
                   max);
  }

  // normalize vertices
  max *= 1.2;
  for (int i = 0; i < vertices.size(); i++) {
    vertices[i].x /= max;
    vertices[i].y /= max;
    vertices[i].z /= max;
  }

  // load faces
  std::vector<Face> faces;
  int a, b, c;
  while (fscanf(fp, "f %d %d %d\n", &a, &b, &c) == 3) {
    faces.push_back(Face(vertices[a - 1], vertices[b - 1], vertices[c - 1]));
  }

  for (float yaw = 0, pitch = 0;; yaw += 0.01, pitch += 0.03) {
    // calculate trig identities
    float yaw_sin = sin(yaw), yaw_cos = cos(yaw), pitch_sin = sin(pitch),
          pitch_cos = cos(pitch);

    // loop through faces
    for (int i = 0; i < faces.size(); i += 1) {
      Face face = faces[i];
      Point3d a = face.a, b = face.b, c = face.c;
      transform_(&a, yaw_sin, yaw_cos, pitch_sin, pitch_cos);
      transform_(&b, yaw_sin, yaw_cos, pitch_sin, pitch_cos);
      transform_(&c, yaw_sin, yaw_cos, pitch_sin, pitch_cos);

      renderer.buffer_polygon(
          {Point(a.x, a.y), Point(b.x, b.y), Point(c.x, c.y)}, LIGHT, NONE);
    }

    renderer.swap_buffer();

    usleep(20000);
  }
}
