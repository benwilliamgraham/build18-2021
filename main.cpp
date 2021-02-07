#include <portaudio.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <sys/types.h>

#define SAMPLE_RATE (120000)
#define WIDTH (80)
#define HEIGHT (80)

typedef struct {
  float left_phase;
  float right_phase;
} paTestData;
/* This routine will be called by the PortAudio engine when audio is needed.
 * It may called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
 */

using namespace cv;

Mat img;
int x = 0, xd = 1, y = 0;

static int patestCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags, void *userData) {
  /* Cast data passed through stream to our structure. */
  paTestData *data = (paTestData *)userData;
  float *out = (float *)outputBuffer;
  unsigned int i;
  (void)inputBuffer; /* Prevent unused variable warning. */

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
    }

    *out++ = data->left_phase;  /* left */
    *out++ = data->right_phase; /* right */

    Scalar b = img.at<u_int8_t>(Point(x, y));
    if (b[0] < 200) {
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
  std::string image_path = samples::findFile("test.png");
  img = imread(image_path, IMREAD_GRAYSCALE);


  // connect to stream
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

  Pa_Sleep(10000);

  Pa_Terminate();
}
