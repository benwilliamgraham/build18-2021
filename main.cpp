#include <portaudio.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include "opencv2/imgproc.hpp"
#include <opencv2/highgui.hpp>
#include "opencv2/opencv.hpp"
#include <sys/types.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <cstdint>
#include <cstring>
#include <vector>

#define SAMPLE_RATE (120000)
#define WIDTH (100)
#define HEIGHT (100)
#define PIXEL_THRESH (80)

typedef struct {
  float left_phase;
  float right_phase;
} paTestData;
/* This routine will be called by the PortAudio engine when audio is needed.
 * It may called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
 */

using namespace cv;

//std::string video_path = "../../Downloads/loop_of_fire_flames.mp4";
std::string video_path = "../../Downloads/mandelbrot.mp4";
VideoCapture cap(video_path);
Mat img;
int x = 0, xd = 1, y = 0;
int Width = 0;
int Height = 0;
int Bpp = 0;

void ImageFromDisplay(std::vector<uint8_t>& Pixels, int& Width, int& Height, int& BitsPerPixel)
{
    Display* display = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(display);

    XWindowAttributes attributes = {0};
    XGetWindowAttributes(display, root, &attributes);

    Width = attributes.width;
    Height = attributes.height;

    XImage* ximg = XGetImage(display, root, 0, 0 , Width, Height, AllPlanes, ZPixmap);
    BitsPerPixel = ximg->bits_per_pixel;
    Pixels.resize(Width * Height * 4);

    memcpy(&Pixels[0], ximg->data, Pixels.size());
    XDestroyImage(ximg);
    XCloseDisplay(display);
}

static int patestCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags, void *userData) {

  /* Cast data passed through stream to our structure. */
  paTestData *data = (paTestData *)userData;
  float *out = (float *)outputBuffer;
  Mat raw_img;
  unsigned int i;
  (void)inputBuffer; /* Prevent unused variable warning. */
  std::vector<std::uint8_t> Pixels;

  for (i = 0; i < framesPerBuffer; i++) {
    // find next dark pixel
    if (xd) {
      x += 1;
      if (x >= WIDTH) {
        xd = 0;
        y += 1;
      }
    } else {
      x -= 1;
      if (x <= 0) {
        xd = 1;
        y += 1;
      }
    }


    if (y >= HEIGHT) {
      y = 0;
      //cap >> raw_img;
      //cvtColor(raw_img, raw_img, CV_BGR2GRAY);
      std::vector<std::uint8_t> Pixels;
      ImageFromDisplay(Pixels, Width, Height, Bpp);
      if (Width && Height)
      {
          Mat raw_img = Mat(Height, Width, Bpp > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]); //Mat(Size(Height, Width), Bpp > 24 ? CV_8UC4 : CV_8UC3, &Pixels[0]);
          cvtColor(raw_img, raw_img, CV_BGR2GRAY);
          resize(raw_img, img, Size(WIDTH,HEIGHT), 0, 0, CV_INTER_LINEAR);
      }
      //resize(raw_img, img, Size(WIDTH,HEIGHT), 0, 0, CV_INTER_LINEAR);
    }


    *out++ = data->left_phase;  /* left */
    *out++ = data->right_phase; /* right */

    Scalar b = img.at<u_int8_t>(Point(x, y));
    if (b[0] < PIXEL_THRESH) {
      continue;
    }

    data->left_phase = 2.0 * (float)x / (float)WIDTH - 1;
    data->right_phase = 2.0 * (float)y / (float)WIDTH - 1;
  }
  return 0;
}


static paTestData data;

int main() {
  // open image
  std::string image_path = "test.png";
  img = imread(image_path, IMREAD_GRAYSCALE);

  PaStream *stream;
  PaError err;

  data.left_phase = data.right_phase = 0.0;

  if (Pa_Initialize() != paNoError) {
    fprintf(stderr, "Cannot initialize pulse audio\n");
    exit(EXIT_FAILURE);
  }

  if (Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, SAMPLE_RATE, 256,
                          patestCallback, &data) != paNoError) {
    fprintf(stderr, "Cannot open stream\n");
    Pa_Terminate();
    exit(EXIT_FAILURE);
  }

  if (Pa_StartStream(stream) != paNoError) {
    fprintf(stderr, "Cannot start stream\n");
    Pa_Terminate();
    exit(EXIT_FAILURE);
  }

  while (1) {
    Pa_Sleep(100000);
  }

  Pa_Terminate();
  cap.release();
  return 0;
}
