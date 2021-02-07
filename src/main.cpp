#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

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

unsigned xPos = 0;
unsigned yPos = 0;
int xDir = 1;
unsigned zoomLvl = 1;

static int patestCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer,
                          const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags, void *userData) {
  /* Cast data passed through stream to our structure. */
  paTestData *data = (paTestData *)userData;
  float *out = (float *)outputBuffer;
  unsigned int i;
  (void)inputBuffer; /* Prevent unused variable warning. */

  // Loop through frames asked for by port audio
  for (i = 0; i < framesPerBuffer; i++) {

    // Find next dark pixel
    if (xDir) {
      xPos++;
      // If x reaches end of the line, swap direction and increment y
      if (xPos >= WIDTH/zoomLvl) {
        xDir = 0;
        yPos++;
      }
    } else {
      xPos--;
      // If x has wrapped around, swap directions and increment y
      if (xPos == 0) {
        xDir = 1;
        yPos++;
      }
    }

    // If finished drawing image, go back to beginning
    if (yPos >= HEIGHT/zoomLvl) {
      yPos = 0;
    }

    // Output values
    *out++ = data->left_phase;  /* left */
    *out++ = data->right_phase; /* right */

    // Get values from image
    Scalar b = img.at<u_int8_t>(Point(xPos, yPos));
    //std::cout << b << " " << xPos << " " << yPos << std::endl;
    if (b[0] < 200) {
      continue;
    }

    // Normalize values
    data->left_phase = 2.0 * (float)xPos / (float)(WIDTH/zoomLvl) - 1;
    data->right_phase = 2.0 * (float)yPos / (float)(HEIGHT/zoomLvl) - 1;
  }

  return 0;
}

static paTestData data;
int main(int argc, char *argv[]) {
  int opt;
  struct termios old_tio, new_tio;
  unsigned char c;
  bool pong_flag = false;
  bool doom_flag = false;
  bool help_flag = false;
  std::string image_path;

  tcgetattr(STDIN_FILENO, &old_tio);

  new_tio = old_tio;

  new_tio.c_lflag &=(~ICANON & ~ECHO);

  tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

  while ((opt = getopt(argc, argv, "hi:pd")) != -1) {
    switch(opt) {
      case 'i':
        image_path = optarg;
        break;
      case 'p':
        pong_flag = true;
        break;
      case 'd':
        doom_flag = true;
        break;
      case 'h':
      default:
        help_flag = true;
        break;
    }
  }

  // open image
  //std::string image_path = "test.png";
  img = imread(image_path, IMREAD_GRAYSCALE);

  // connect to stream
  PaStream *stream;
  PaError err;

  data.left_phase = data.right_phase = 0.0;

  if (Pa_Initialize() != paNoError) {
    fprintf(stderr, "Cannot initialize pulse audio\n");
    goto done;
  }

  if (Pa_OpenDefaultStream(&stream, 0, 2, paFloat32, SAMPLE_RATE, 256,
                           patestCallback, &data) != paNoError) {
    fprintf(stderr, "Cannot open stream\n");
    Pa_Terminate();
    goto done;
  }

  if (Pa_StartStream(stream) != paNoError) {
    fprintf(stderr, "Cannot start stream\n");
    Pa_Terminate();
    goto done;
  }

  while(1) {
    char c;
    c = getchar();
    switch (c) {
      case 'q':
        goto done;
        break;
      case 'i':
        zoomLvl++;
        break;
      case 'o':
        zoomLvl--;
        break;
      case 'w':
        break;
      case 'a':
        break;
      case 's':
        break;
      case 'd':
        break;
    }
    if (c == 'q') {
      goto done;
    }
  }

done:
  tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
  Pa_Terminate();
}
