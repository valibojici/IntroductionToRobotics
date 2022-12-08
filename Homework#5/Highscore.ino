void resetHighscores() {
  const int delay = 2000;
  static bool reseted = false;
  static unsigned long timer = 0;
  if (!reseted) {
    timer = millis();
    reseted = true;
    gameSettings.highscoresCount = 0;
    saveToEEPROM();
    printLcdLines("Highscores", "reseted!", false);
    timer = millis();
  }

  if (millis() - timer > delay) {
    reseted = false;
    gameState = SETTINGS;
  }
}

bool isNewHighscore() {
  if (gameSettings.highscoresCount < MAX_HIGHSCORES_COUNT) {
    return true;
  }
  return gameScore > gameSettings.highscores[MAX_HIGHSCORES_COUNT - 1].score;
}

void insertNewHighscore(const String& name) {
  int& highscoresCount = gameSettings.highscoresCount;

  if (highscoresCount == 0) {
    strcpy(gameSettings.highscores[highscoresCount].name, name.c_str());
    gameSettings.highscores[highscoresCount].score = gameScore;
    highscoresCount++;
    saveToEEPROM();
    return;
  }
  
  byte position = MAX_HIGHSCORES_COUNT - 1;
  if (highscoresCount < MAX_HIGHSCORES_COUNT) {
    position = highscoresCount;  // 1 after the end
    highscoresCount++;
  }
  while (position > 0 && gameSettings.highscores[position - 1].score < gameScore) {
    strcpy(gameSettings.highscores[position].name, gameSettings.highscores[position - 1].name);
    gameSettings.highscores[position].score = gameSettings.highscores[position - 1].score;
    position--;
  }
  strcpy(gameSettings.highscores[position].name, name.c_str());
  gameSettings.highscores[position].score = gameScore;

  saveToEEPROM();
}

void newHighscore() {
  static bool menuChanged = true;
  static String name = "AAA";
  static byte cursorPos = 0;
  const int letterPos = 11;

  if (menuChanged) {
    menuChanged = false;
    printLcdLines("New highscore!", getNewHighscoreText(name).c_str(), false);
    if (cursorPos >= 0 && cursorPos <= 2) {
      lcd.setCursor(cursorPos + letterPos, 1);
    } else {
      lcd.setCursor(15, 1);
    }

    lcd.blink();
  }

  if (joyState == JOY_LEFT && cursorPos > 0) {
    cursorPos--;
    menuChanged = true;
  } else if (joyState == JOY_RIGHT && cursorPos < 3) {
    cursorPos++;
    menuChanged = true;
  } else if ((joyState == JOY_UP || joyState == JOY_DOWN) && cursorPos != 3) {
    char letter = name.charAt(cursorPos);
    if (joyState == JOY_UP) {
      letter = letter + 1 > 'Z' ? 'A' : letter + 1;
    } else {
      letter = letter - 1 < 'A' ? 'Z' : letter - 1;
    }
    name.setCharAt(cursorPos, letter);
    menuChanged = true;
  }

  if (joyPress && cursorPos == 3) {
    insertNewHighscore(name);

    lcd.noBlink();
    menuChanged = true;
    cursorPos = 0;
    name = String("AAA");
    gameState = MAINMENU;
  }
}

String getNewHighscoreText(String name) {
  return String("Enter name ") + name + String(" >");
}