#pragma once
#include "Player.h"

struct Settings {
  int lcdContrast;
  int lcdBrightness;
  int difficulty;
  int matrixBrightness;
  int soundIsEnabled;
  Player highscores[MAX_HIGHSCORES_COUNT];
  int highscoresCount;
};