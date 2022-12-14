#pragma once

const byte pinX = A0;  // A0 - analog pin connected to X output
const byte pinY = A1;  // A1 - analog pin connected to Y output
const byte swPin = 0;
const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 3;
const byte d7 = 4;
const byte contrastPin = 5;
const byte backlightPin = 10;
const byte dinPin = 13;
const byte clockPin = 12;
const byte loadPin = 11;
const byte buzzerPin = A2;

const byte MATRIX_SIZE = 8;
const byte MAX_LCD_CONTRAST_OUTPUT = 150;
const byte MIN_LCD_CONTRAST_OUTPUT = 40;
const byte MAX_LCD_BRIGHTNESS_OUTPUT = 255;
const byte MIN_LCD_BRIGHTNESS_OUTPUT = 50;

const byte MAX_DIFFICULTY = 6;
const byte MAX_LCD_CONTRAST = 8;
const byte MAX_LCD_BRIGHTNESS = 8;
const byte MAX_MATRIX_BRIGHTNESS = 7;
const byte MAX_HIGHSCORES_COUNT = 5;

const int MENU_SELECT_FREQUENCY = 260;
const int MENU_BACK_FREQUENCY = 200;
const int MENU_FORWARD_FREQUENCY = 300;
const int MENU_BUZZER_DURATION = 75;
const int SNAKE_BUZZER_FREQUENCY = 180;
const int SNAKE_BUZZER_DURATION = 27;
const int FOOD_BUZZER_FREQUENCY = 360;

const int MAX_GAME_DELAY = 400;
const int MIN_GAME_DELAY = 110;
const int FOOD_DELAY = 500;
const int SNAKE_DELAY = 27;

const int MIN_X_THRESHOLD = 200;
const int MAX_X_THRESHOLD = 800;
const int MIN_Y_THRESHOLD = 200;
const int MAX_Y_THRESHOLD = 800;

const int LCD_SCROLL_DELAY = 350;
const byte LCD_OPTION_MAX_LENGTH = 15;

typedef void (*GameStateFunction)();

const byte arrow[8] = {
  0b11000,
  0b11100,
  0b11110,
  0b11111,
  0b11110,
  0b11100,
  0b11000,
  0b00000
};

enum GameState {
  INTRO,
  MAINMENU,
  SETTINGS,
  SETTINGS_MATRIX_BRIGHT,
  SETTINGS_LCD_CONTRAST,
  SETTINGS_LCD_BRIGHT,
  SETTINGS_DIFFICULTY,
  SETTINGS_SOUND,
  ABOUT,
  HOWTOPLAY,
  HIGHSCORES,
  GAMEOVER,
  HIGHSCORES_NEW,
  HIGHSCORES_RESET,
  GAMERUNNING,
};

enum JoyMovements {
  JOY_LEFT,
  JOY_UP,
  JOY_DOWN,
  JOY_RIGHT,
  NONE
};

const char* mainMenuText[] = {
  "Start game",
  "Highscores",
  "Settings",
  "About",
  "How to play",
};
const byte MAIN_MENU_LENGTH = sizeof(mainMenuText) / sizeof(char*);

const char* settingsMenuText[] = {
  "Difficulty",
  "LCD contrast",
  "LCD brightness",
  "Matrix brightness",
  "Sounds",
  "Reset highscores",
};
const byte SETTINGS_MENU_LENGTH = sizeof(settingsMenuText) / sizeof(char*);

const char* aboutMenuText[] = {
  "Game: Snake",
  "Author: Valentin Bojici",
  "Github: github.com/valibojici/IntroductionToRobotics"
};
const byte ABOUT_MENU_LENGTH = sizeof(aboutMenuText) / sizeof(char*);

const char* howToPlayText = "Joystick to move. Collect food to increase score.";
const char* noHighscoresText = "No highscores";

const int matrixMenuImages[][8] = {
  { 0b00000000,
    0b00100100,
    0b00100100,
    0b00000000,
    0b10000001,
    0b01000010,
    0b00111100,
    0b00000000 },
  { 0b00011000,
    0b00111100,
    0b01111110,
    0b11111111,
    0b01000010,
    0b01011010,
    0b01011010,
    0b01111110 },
  { 0b00100100,
    0b00100100,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00100100,
    0b00100100 },
  { 0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111,
    0b11111111 },
  { 0b00111100,
    0b01110010,
    0b11110001,
    0b11110001,
    0b11110001,
    0b11110001,
    0b01110010,
    0b00111100 },
  { 0b10000001,
    0b01000010,
    0b00011000,
    0b00111100,
    0b00111100,
    0b00011000,
    0b01000010,
    0b10000001 },
  { 0b00000010,
    0b00000010,
    0b00001010,
    0b00001010,
    0b00001010,
    0b00101010,
    0b00101010,
    0b10101010 },
  { 0b00000000,
    0b00100100,
    0b01101010,
    0b11100101,
    0b11100101,
    0b01101010,
    0b00100100,
    0b00000000 },
  { 0b00011000,
    0b00011000,
    0b00000000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000,
    0b00011000 },
  { 0b00111100,
    0b01111110,
    0b01000110,
    0b01001100,
    0b00011000,
    0b00011000,
    0b00000000,
    0b00011000 },
  { 0b00000000,
    0b10000001,
    0b11111111,
    0b10111101,
    0b01111110,
    0b00011000,
    0b00011000,
    0b00111100 },
  { 0b00000000,
    0b00100100,
    0b00100100,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b10000001 },
  { 0b00000000,
    0b10000001,
    0b11111111,
    0b10111101,
    0b01111110,
    0b00011000,
    0b00011000,
    0b00111100 },
  { 0b00000000,
    0b10000001,
    0b11111111,
    0b10111101,
    0b01111110,
    0b00011000,
    0b00011000,
    0b00111100 }
};