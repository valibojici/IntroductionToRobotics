unsigned long lcdTimer = 0;
unsigned long buzzerTimer = 0;
int scroll = 0;
int maxScroll = 0;
int scrollDirection = 1;

void intro() {
  static int delay = 2000;
  static unsigned long start = millis();
  static bool showedIntro = false;
  if (!showedIntro) {
    printLcdLines("Hello! You're", "playing Snake", false);
    showedIntro = true;
  }
  if (millis() - start > delay) {
    gameState = MAINMENU;
  }
}

void mainMenu() {
  static bool menuChanged = true;
  static int currentOption = 0;
  static GameState states[] = { GAMERUNNING, HIGHSCORES, SETTINGS, ABOUT, HOWTOPLAY };
  changeOption(currentOption, MAIN_MENU_LENGTH, menuChanged);

  String nextLine = currentOption + 1 < MAIN_MENU_LENGTH ? mainMenuText[currentOption + 1] : "";
  printWithScroll(mainMenuText[currentOption], nextLine, menuChanged, nullptr, true);

  forwardOption(states[currentOption], menuChanged, nullptr);
}

void settingsMenu() {
  static bool menuChanged = true;
  static int currentOption = 0;
  static GameState states[] = {
    SETTINGS_DIFFICULTY,
    SETTINGS_LCD_CONTRAST,
    SETTINGS_LCD_BRIGHT,
    SETTINGS_MATRIX_BRIGHT,
    SETTINGS_SOUND,
    HIGHSCORES_RESET,
  };
  changeOption(currentOption, SETTINGS_MENU_LENGTH, menuChanged);

  String nextLine = currentOption + 1 < SETTINGS_MENU_LENGTH ? settingsMenuText[currentOption + 1] : "";
  printWithScroll(settingsMenuText[currentOption], nextLine, menuChanged, nullptr, true);

  forwardOption(states[currentOption], menuChanged, nullptr);
  backOption(MAINMENU, menuChanged, nullptr);
}

void aboutMenu() {
  static bool menuChanged = true;
  static int currentOption = 0;

  changeOption(currentOption, ABOUT_MENU_LENGTH, menuChanged);

  String nextLine = currentOption + 1 < ABOUT_MENU_LENGTH ? aboutMenuText[currentOption + 1] : "";
  printWithScroll(aboutMenuText[currentOption], nextLine, menuChanged, nullptr, true);

  backOption(MAINMENU, menuChanged, nullptr);
}

void settingsSubMenu() {
  static bool menuChanged = true;
  int* currentValue;
  int maxValue;

  switch (gameState) {
    case SETTINGS_DIFFICULTY:
      currentValue = &(gameSettings.difficulty);
      maxValue = MAX_DIFFICULTY;
      break;
    case SETTINGS_LCD_BRIGHT:
      currentValue = &(gameSettings.lcdBrightness);
      maxValue = MAX_LCD_BRIGHTNESS;
      break;
    case SETTINGS_LCD_CONTRAST:
      currentValue = &(gameSettings.lcdContrast);
      maxValue = MAX_LCD_CONTRAST;
      break;
    case SETTINGS_MATRIX_BRIGHT:
      currentValue = &(gameSettings.matrixBrightness);
      maxValue = MAX_MATRIX_BRIGHTNESS;
      break;
    case SETTINGS_SOUND:
      currentValue = &(gameSettings.soundIsEnabled);
      maxValue = 2;
      break;
  }

  if ((joyState == JOY_DOWN && *currentValue != 0) || (joyState == JOY_UP && *currentValue != maxValue - 1)) {
    menuChanged = true;
    *currentValue = joyState == JOY_DOWN ? *currentValue - 1 : *currentValue + 1;
  }

  String line;
  if (gameState == SETTINGS_SOUND) {
    line = (*currentValue == 0 ? "OFF" : "ON");
  } else {
    line = "-";
    for (int i = 0; i < maxValue; ++i) {
      // \xff = full block \xa0 = empty block
      line += (i <= *currentValue ? '\xff' : '\xa0');
    }
    line += '+';
  }
  printWithScroll(line, "", menuChanged, []() {
    updateSettings();
  },false);

  backOption(SETTINGS, menuChanged, []() {
    saveToEEPROM();
  });
}

void howToPlayMenu() {
  static bool menuChanged = true;
  printWithScroll(howToPlayText, "", menuChanged, nullptr, false);
  backOption(MAINMENU, menuChanged, nullptr);
}

void highscoresMenu() {
  static bool menuChanged = true;
  static int currentOption = 0;
  
  if (gameSettings.highscoresCount <= 0) {
    printWithScroll(noHighscoresText, "", menuChanged, nullptr, false);
  } else {
    currentOption = min(currentOption, gameSettings.highscoresCount - 1);
    changeOption(currentOption, gameSettings.highscoresCount, menuChanged);
    
    String players[2] = { String(currentOption + 1) + ". ", "" };
    players[0] += String(gameSettings.highscores[currentOption].name) + " " + String(gameSettings.highscores[currentOption].score);
    
    if (currentOption + 1 < gameSettings.highscoresCount) {
      players[1] = String(currentOption + 2) + ". ";
      players[1] += String(gameSettings.highscores[currentOption + 1].name) + " " + String(gameSettings.highscores[currentOption + 1].score);
    }
    printWithScroll(players[0], players[1], menuChanged, nullptr, false);
  }
  backOption(MAINMENU, menuChanged, nullptr);
}

void gameOverMenu() {
  const int delay = 2000;
  static byte state = 0;
  static bool menuChanged = true;
  static unsigned long timer = 0;

  String lines[2];

  if (state == 0) {
    state = 1;
    timer = millis();
    lines[0] = "Game over!";
    lines[1] = "";
    menuChanged = true;
  } else if (state == 1) {
    if (millis() - timer > delay) {
      menuChanged = true;
      lines[0] = "Score: " + String(gameScore);
      lines[1] = "Time: " + String(formatTime(millis() - gameStart));
      state = 2;
    }
  } else if (state == 2) {
    if (joyPress) {
      state = 0;
      gameState = isNewHighscore() ? HIGHSCORES_NEW : MAINMENU;
    }
  }

  if (menuChanged) {
    menuChanged = false;
    printLcdLines(lines[0].c_str(), lines[1].c_str(), false);
  }
}

String formatTime(unsigned long time) {
  time = time / 1000;
  if (time < 60) {
    return String(time) + "s";
  }
  return String(time / 60) + "m" + String(time % 60) + "s";
}


void backOption(GameState state, bool& menuChanged, void (*callback)()) {
  if (joyState == JOY_LEFT) {
    gameState = state;
    menuChanged = true;
    scroll = 0;
    scrollDirection = 1;
    menuSelectSound(MENU_BACK_FREQUENCY, MENU_BUZZER_DURATION);
    if (callback) {
      callback();
    }
  }
}

void forwardOption(GameState state, bool& menuChanged, void (*callback)()) {
  if (joyState == JOY_RIGHT) {
    gameState = state;
    menuChanged = true;
    scroll = 0;
    scrollDirection = 1;
    menuSelectSound(MENU_SELECT_FREQUENCY, MENU_BUZZER_DURATION);
    if (callback) {
      callback();
    }
  }
}

void changeOption(int& currentOption, int maxVal, bool& menuChanged) {
  if ((joyState == JOY_DOWN && currentOption != maxVal - 1) || (joyState == JOY_UP && currentOption != 0)) {
    currentOption = joyState == JOY_DOWN ? currentOption + 1 : currentOption - 1;
    menuChanged = true;
    scroll = 0;
    scrollDirection = 1;
    menuSelectSound(MENU_SELECT_FREQUENCY, MENU_BUZZER_DURATION);
    lcdTimer = millis();
  }
}

void printWithScroll(const String& top, const String& bottom, bool& menuChanged, void (*menuChangeCallback)(void), bool isSelected) {
  if (menuChanged) {
    menuChanged = false;

    if (top.length() > LCD_OPTION_MAX_LENGTH) {
      maxScroll = top.length() - LCD_OPTION_MAX_LENGTH;
      top = top.substring(scroll, LCD_OPTION_MAX_LENGTH + scroll);
    } else {
      maxScroll = 0;
    }

    if (bottom.length() > LCD_OPTION_MAX_LENGTH) {
      bottom = bottom.substring(0, LCD_OPTION_MAX_LENGTH);
    }

    printLcdLines(top.c_str(), bottom.c_str(), isSelected);
    if (menuChangeCallback) {
      menuChangeCallback();
    }
    lcdTimer = millis();
  }

  if (maxScroll && millis() - lcdTimer > LCD_SCROLL_DELAY + LCD_SCROLL_DELAY * (scroll == maxScroll || scroll == 0)) {
    scroll += scrollDirection;
    if (scroll == maxScroll || scroll == 0) {
      scrollDirection *= -1;
    }
    menuChanged = true;
  }
}

void printLcdLines(const char* top, const char* bottom, bool isSelected) {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (isSelected) {
    lcd.write((byte)0);
  }
  lcd.write(top);
  lcd.setCursor(0, 1);
  lcd.write(bottom);
}

void menuSelectSound(unsigned int frequency, unsigned long duration) {
  if (gameSettings.soundIsEnabled) {
    tone(buzzerPin, frequency, duration);
  }
}