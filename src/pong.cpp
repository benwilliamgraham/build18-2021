#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "pong.h"


namespace pong{

struct {
  int speed;
  int ballDirX;
  int ballDirY;
} gameState;

void moveAndOpp() {
  // Move Ball
  pongData.bposX += gameState.ballDirX;
  pongData.bposY += gameState.ballDirY;

  // Move opponents
  if (pongData.rposY < pongData.bposY) {
    pongData.rposY += 1;
  } else {
    pongData.rposY -= 1;
  }
}

void bounce() {
  // Check the ceiling and floor
  if (pongData.bposY >= pongData.height) {
    gameState.ballDirY = -1;
  } else if (pongData.bposY <= 0) {
    gameState.ballDirY = 1;
  }

  // Check if the left paddle hits the ball
  if (pongData.lposX <= pongData.bposX &&
       pongData.bposX <= pongData.lposX + pongData.paddleWidth) {
    if (pongData.lposY <= pongData.bposY &&
       pongData.bposY <= pongData.lposY + pongData.paddleHeight) {
      gameState.ballDirX = 1;
    }
  }

  // Check if the right paddle hits the ball
  if (pongData.rposX <= pongData.bposX &&
       pongData.bposX <= pongData.rposX + pongData.paddleWidth) {
    if (pongData.rposY <= pongData.bposY &&
       pongData.bposY <= pongData.rposY + pongData.paddleHeight) {
      gameState.ballDirX = -1;
    }
  }
}

bool checkOOB() {
  return (pongData.bposX < 0) || (pongData.bposX > pongData.width);
}

void run() {
  // init
  while (1) {
    gameState.ballDirX = 1;
    gameState.ballDirY = 1;
    gameState.speed    = 1;

    char c;
    c = getchar();
    if (c == 'b') {
      break;
    }
  }

  while(1) {
    char c;
    c = getchar();
    switch(c) {
      case 'w': {
        if (pongData.lposY < pongData.height) {
          pongData.lposY += 1;
        }
        break;
      }
      case 's': {
        if (pongData.lposY > 0) {
          pongData.lposY -= 1;
        }
        break;
      }
    }

    // Move the AI
    moveAndOpp();

    bounce();

    // Check OOB
    if (checkOOB()) {
      pongData.bposY = pongData.height / 2;
      pongData.bposX = pongData.width / 2;
    }

  }
}

}; // pong