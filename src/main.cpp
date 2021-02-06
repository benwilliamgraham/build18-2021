#include <portaudio.h>
#include <stdio.h>
#include <stdlib.h>

#define SAMPLE_RATE (44100)

typedef struct {
  float left_phase;
  float right_phase;
} paTestData;
/* This routine will be called by the PortAudio engine when audio is needed.
 * It may called at interrupt level on some machines so don't do anything
 * that could mess up the system like calling malloc() or free().
 */

int dir = 0;

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
    *out++ = data->left_phase;  /* left */
    *out++ = data->right_phase; /* right */
    switch (i % 4) {
    case 0:
      data->left_phase = -1;
      data->right_phase = -1;
      break;
    case 1:
      data->left_phase = -1;
      data->right_phase = 1;
      break;
    case 2:
      data->left_phase = 1;
      data->right_phase = 1;
      break;
    case 3:
      data->left_phase = 1;
      data->right_phase = -1;
      break;
    }
  }
  return 0;
}

static paTestData data;
int main() {
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
