const int latchPin = 11;  // STCP to 12 on Shift Register
const int clockPin = 10;  // SHCP to 11 on Shift Register
const int dataPin = 12;   // DS to 14 on Shift Register
const int pinSW = 2;      // digital pin connected to switch output
const int pinX = A0;      // A0 - analog pin connected to X output
const int pinY = A1;      // A1 - analog pin connected to Y output

const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const int displayCount = 4;
const int multiplexingDelay = 5;
int displayDigits[] = {
  segD1, segD2, segD3, segD4
};

int displayValues[] = { 0, 0, 0, 0 };
int currentPosition = displayCount - 1;
int systemState = 1;
volatile byte swState = LOW;
bool swPressed = false;
bool isLongPress = false;
unsigned long swDebounceDelay = 50;
unsigned long swPressTime;
int swHoldDelay = 2000;
int xValue = 0;
int yValue = 0;
bool joyMoved = false;
int joyState;
int minXThreshold = 200;
int maxXThreshold = 800;
int minYThreshold = 200;
int maxYThreshold = 800;
byte dpState = HIGH;
unsigned long dpDelay = 300;
int maxDisplayValue = 15;
int minDisplayValue = 0;

enum JoyMovements { JOY_LEFT,
                    JOY_UP,
                    JOY_DOWN,
                    JOY_RIGHT,
                    NONE };

byte byteEncodings[] = {
  //A B C D E F G DP
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110,  // 9
  B11101110,  // A
  B00111110,  // b
  B10011100,  // C
  B01111010,  // d
  B10011110,  // E
  B10001110   // F
};


void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(pinSW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(pinSW), swPress, CHANGE);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], HIGH);
  }
}

void loop() {
  executeState();
  writeOutput();
}

void executeState() {
  if (systemState == 1) {
    joyState = getJoyMovement();
    switch (joyState) {
      case JOY_LEFT:
        currentPosition = (currentPosition == 0 ? displayCount - 1 : currentPosition - 1);
        break;
      case JOY_RIGHT:
        currentPosition = (currentPosition == displayCount - 1 ? 0 : currentPosition + 1);
        break;
    }
    if (swState && isLongPress) {
      return;
    }
    isLongPress = false;
    if (swState && !swPressed) {
      swPressed = true;
      swPressTime = millis();
    }
    if (swPressed && millis() - swPressTime > swHoldDelay) {
      resetDisplay();
      isLongPress = true;
      swPressed = false;
      return;
    }
    if (!swState && swPressed) {
      swPressed = false;
      systemState = 2;
    }
  }

  if (systemState == 2) {
    joyState = getJoyMovement();
    switch (joyState) {
      case JOY_UP:
        displayValues[currentPosition] = displayValues[currentPosition] + 1;
        break;
      case JOY_DOWN:
        displayValues[currentPosition] = displayValues[currentPosition] - 1;
        break;
    }
    if (displayValues[currentPosition] < minDisplayValue) {
      displayValues[currentPosition] = maxDisplayValue;
    }
    if (displayValues[currentPosition] > maxDisplayValue) {
      displayValues[currentPosition] = minDisplayValue;
    }
    if (swState && !swPressed) {
      swPressed = true;
    }
    if (!swState && swPressed) {
      systemState = 1;
      swPressed = false;
    }
  }
}

void resetDisplay() {
  for (int i = 0; i < displayCount; ++i) {
    displayValues[i] = 0;
  }
  dpState = HIGH;
  currentPosition = displayCount - 1;
}

JoyMovements getJoyMovement() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  if (xValue > minXThreshold && xValue < maxXThreshold && yValue > minYThreshold && yValue < maxYThreshold) {
    joyMoved = false;
  }

  if (!joyMoved && xValue < minXThreshold) {
    joyMoved = true;
    return JOY_LEFT;
  }
  if (!joyMoved && xValue > maxXThreshold) {
    joyMoved = true;
    return JOY_RIGHT;
  }
  if (!joyMoved && yValue < minYThreshold) {
    joyMoved = true;
    return JOY_UP;
  }
  if (!joyMoved && yValue > maxXThreshold) {
    joyMoved = true;
    return JOY_DOWN;
  }
  return NONE;
}

void writeArray(int array[], int dpPosition = -1) {
  static byte activeDisplay = 0;
  static unsigned long lastDisplayTime = 0;
  if (millis() - lastDisplayTime > multiplexingDelay) {
    lastDisplayTime = millis();
    digitalWrite(displayDigits[activeDisplay], HIGH);
    activeDisplay = (activeDisplay + 1) % displayCount;
  }
  if (dpPosition != displayCount - (activeDisplay + 1)) {
    writeReg(byteEncodings[array[displayCount - (activeDisplay + 1)]]);
  } else {
    writeReg(byteEncodings[array[displayCount - (activeDisplay + 1)]] | 1);
  }

  digitalWrite(displayDigits[activeDisplay], LOW);
}

void writeOutput() {
  static unsigned long lastDpTime = 0;

  if (systemState == 1) {
    if (millis() - lastDpTime > dpDelay) {
      lastDpTime = millis();
      dpState = !dpState;
    }
    writeArray(displayValues, dpState ? currentPosition : -1);
  }
  if (systemState == 2) {
    writeArray(displayValues, currentPosition);
  }
}

void swPress(){
  static unsigned long swDebounceTime = 0;
  if(millis() - swDebounceTime > swDebounceDelay){
    swState = !swState;
  }
  swDebounceTime = millis();
}

void writeReg(int encoding) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, encoding);
  digitalWrite(latchPin, HIGH);
}
