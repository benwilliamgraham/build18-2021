#include "oscopegl.hpp"
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <cstring>

using namespace oscopegl;

#define SAMPLE_RATE 9600
#define DIM 100


int neighbors (int x, int y)

int main() {
  Renderer *R = Renderer::instance();
  R->start(SAMPLE_RATE);
  int pixel_arr[DIM][DIM] = {};
  int next_arr[DIM][DIM] = {};
  pixel_arr[50][50] = 1;
  pixel_arr[50][51] = 1;
  pixel_arr[50][49] = 1;
  pixel_arr[49][50] = 1;

  for (int n = 0;; n++) {
    R->clear();

    for(int i = 0; i < DIM; i++ )
    {
      for (int j = 0; j < DIM; j++)
      {

      int sum = 0;

      if (0 <= i-1 && i-1 < DIM){
        sum += pixel_arr[i-1][j];
      }
      if (0 <= i+1 && i+1 < DIM){
        sum += pixel_arr[i+1][j];
      }
      if (0 <= j-1 && j-1 < DIM){
        sum += pixel_arr[i][j-1];
      }
      if (0 <= j+1 && j+1 < DIM){
        sum += pixel_arr[i][j+1];
      }

      if (0 <= i-1 && i-1 < DIM && 0 <= j-1 && j-1 < DIM){
        sum += pixel_arr[i-1][j-1];
      }
      if (0 <= i+1 && i+1 < DIM && 0 <= j-1 && j-1 < DIM){
        sum += pixel_arr[i+1][j-1];
      }
      if (0 <= i-1 && i-1 < DIM && 0 <= j+1 && j+1 < DIM){
        sum += pixel_arr[i-1][j+1];
      }
      if (0 <= i+1 && i+1 < DIM && 0 <= j+1 && j+1 < DIM){
        sum += pixel_arr[i+1][j+1];
      }

      if (pixel_arr[i][j]){
        next_arr[i][j] = (sum == 2 || sum == 3) ? 1 : 0;
        buffer_point(Point(0.01*i, 0.01*j), DARK);
      }
      else {
        next_arr[i][j] = (sum == 3) ? 1 : 0;
      }

      }
    }
    memcpy(pixel_arr, next_arr, DIM*DIM*sizeof(int));
    memset(next_arr, 0, DIM*DIM*sizeof(int));
    R->swap_buffer();

    sleep(100);
  }
}
