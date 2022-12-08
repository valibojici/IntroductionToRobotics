byte gameMatrix[][MATRIX_SIZE] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
};

unsigned long gameTimer = 0;
unsigned long foodTimer = 0;
unsigned long snakeTimer = 0;
byte snakeHistory[64];
byte snakeHistoryLength;
int gameDelay;
bool gameStarted = false;
byte difficulty;
bool gameInfoUpdated;

byte rowSnake;
byte colSnake;
byte rowFood;
byte colFood;

int rowDir;
int colDir;

bool foodExists;
bool matrixChanged;
bool gameInfoChanged;

void runGame() {
  if (!gameStarted) {
    initGame();
  }

  if (!foodExists) {
    generateFood();
  }
  handleInput();

  foodBlink();
  snakeBlink();

  if (millis() - gameTimer > gameDelay) {
    updateSnakePosition();
    gameTimer = millis();
    matrixChanged = true;
  }

  drawMatrix();
  printGameInfo();
}

void foodBlink() {
  if (millis() - foodTimer > FOOD_DELAY) {
    gameMatrix[rowFood][colFood] = 1 - gameMatrix[rowFood][colFood];
    foodTimer = millis();
    matrixChanged = true;
  }
}

void snakeBlink() {
  if (millis() - snakeTimer > SNAKE_DELAY) {
    gameMatrix[rowSnake][colSnake] = 1 - gameMatrix[rowSnake][colSnake];
    snakeTimer = millis();
    matrixChanged = true;
  }
}

void generateFood() {
  gameMatrix[rowFood][colFood] = 0;
  rowFood = random(0, MATRIX_SIZE - 1);
  colFood = random(0, MATRIX_SIZE - 1);
  while (checkInHistory(rowFood, colFood)) {
    rowFood = random(0, MATRIX_SIZE - 1);
    colFood = random(0, MATRIX_SIZE - 1);
  }
  foodExists = true;
}

void processFoodCollect() {
  foodExists = false;
  gameScore += (difficulty + 1) * 10 + snakeHistoryLength * 5;
  gameInfoUpdated = true;
  if (snakeHistoryLength % 3 == 0) {
    // increase difficulty
    difficulty = min(difficulty + 1, MAX_DIFFICULTY - 1);
    gameDelay = map(difficulty, 0, MAX_DIFFICULTY - 1, MAX_GAME_DELAY, MIN_GAME_DELAY);
    gameInfoUpdated = true;
  }
}

void updateSnakePosition() {
  historyPush(rowSnake, colSnake);
  if (rowSnake == 0 && rowDir == -1) {
    rowSnake = MATRIX_SIZE - 1;
  } else if (rowSnake == MATRIX_SIZE - 1 && rowDir == 1) {
    rowSnake = 0;
  } else {
    rowSnake += rowDir;
  }

  if (colSnake == 0 && colDir == -1) {
    colSnake = MATRIX_SIZE - 1;
  } else if (colSnake == MATRIX_SIZE - 1 && colDir == 1) {
    colSnake = 0;
  } else {
    colSnake += colDir;
  }

  if (rowSnake == rowFood && colSnake == colFood) {
    processFoodCollect();
    if (gameSettings.soundIsEnabled) {
      tone(buzzerPin, FOOD_BUZZER_FREQUENCY, SNAKE_BUZZER_DURATION);
    }
  } else {
    historyPop();
    if (gameSettings.soundIsEnabled) {
      tone(buzzerPin, SNAKE_BUZZER_FREQUENCY, SNAKE_BUZZER_DURATION);
    }
  }

  if (checkInHistory(rowSnake, colSnake) || snakeHistoryLength == 64) {
    gameStarted = false;
    gameState = GAMEOVER;
  }
}

void initGame() {
  difficulty = constrain(gameSettings.difficulty, 0, MAX_DIFFICULTY - 1);
  gameDelay = map(difficulty, 0, MAX_DIFFICULTY - 1, MAX_GAME_DELAY, MIN_GAME_DELAY);
  snakeHistoryLength = 0;
  for (int i = 0; i < difficulty / 2; ++i) {
    historyPush(0, i);
  }
  colSnake = difficulty / 2;
  rowSnake = 0;
  gameMatrix[rowSnake][colSnake] = 1;
  rowDir = 0;
  colDir = 1;
  gameScore = 0;
  gameInfoUpdated = true;
  foodExists = false;
  matrixChanged = true;
  gameInfoChanged = true;
  gameStart = millis();
  gameStarted = true;
  lcd.clear();
}

void printGameInfo() {
  if (gameInfoUpdated) {
    gameInfoUpdated = false;
    String top = "Score: " + String(gameScore);
    String bottom = "Difficulty: " + String(difficulty);
    printLcdLines(top.c_str(), bottom.c_str(), false);
  }
}

void drawMatrix() {
  if (!matrixChanged) {
    return;
  }
  matrixChanged = false;

  byte foodValue = gameMatrix[rowFood][colFood];
  byte snakeValue = gameMatrix[rowSnake][colSnake];

  resetMatrix();

  for (int i = 0; i < snakeHistoryLength; ++i) {
    byte row = (snakeHistory[i] & '\xf0') >> 4;
    byte col = snakeHistory[i] & '\x0f';
    gameMatrix[row][col] = 1;
  }
  gameMatrix[rowSnake][colSnake] = snakeValue;
  gameMatrix[rowFood][colFood] = foodValue;
  lightMatrix(gameMatrix);
}

void resetMatrix() {
  for (int i = 0; i < MATRIX_SIZE; ++i) {
    for (int j = 0; j < MATRIX_SIZE; ++j)
      gameMatrix[i][j] = 0;
  }
}