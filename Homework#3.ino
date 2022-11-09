// declare all the joystick pins
const int pinSW = 2;  // digital pin connected to switch output
const int pinX = A0;  // A0 - analog pin connected to X output
const int pinY = A1;  // A1 - analog pin connected to Y output
// declare all the segments pins
const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;
const int segSize = 8;

byte swReading;
byte swLastReading;
byte swState = LOW;
bool swPressed = false;
bool isLongPress = false;
unsigned long swDebounceDelay = 50;
unsigned long swDebounceTime = 0;
unsigned long swPressTime;
int swHoldDelay = 1000;
int xValue = 0;
int yValue = 0;
bool joyMoved = false;
int joyState;
int minXThreshold = 200;
int maxXThreshold = 800;
int minYThreshold = 200;
int maxYThreshold = 800;
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
int activeSegment = 0;
byte activeSegmentState = HIGH;
unsigned long activeSegmentTime = 0;
unsigned long activeSegmentDelay = 200;
int segmentNeighborIndex;
int segmentNeighbors[segSize][4] = {
  // left, right, up, down
  { 5, 1, -1, 6 },   // 0
  { 5, -1, 0, 6 },   // 1
  { 4, 7, 6, 3 },    // 2
  { 4, 2, 6, -1 },   // 3
  { -1, 2, 6, 3 },   // 4
  { -1, 1, 0, 6 },   // 5
  { -1, -1, 0, 3 },  // 6
  { 2, -1, -1, -1 },
};
byte segmentStates[segSize];
byte systemState = 1;

enum JoyMovements { JOY_LEFT,
                    JOY_UP,
                    JOY_DOWN,
                    JOY_RIGHT,
                    NONE };

void setup() {
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  for (int i = 0; i < segSize; ++i) {
    segmentStates[i] = LOW;
  }
  pinMode(pinSW, INPUT_PULLUP);
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
        activeSegment = getNextSegment(-1, 0);
        break;
      case JOY_RIGHT:
        activeSegment = getNextSegment(1, 0);
        break;
      case JOY_UP:
        activeSegment = getNextSegment(0, 1);
        break;
      case JOY_DOWN:
        activeSegment = getNextSegment(0, -1);
        break;
    }
    swState = getButtonState();

    if(swState && isLongPress){
      return;
    }
    isLongPress = false;
    if (swState && !swPressed) {
      swPressed = true;
      swPressTime = millis();
    }
    if (swPressed && millis() - swPressTime > swHoldDelay) {
        resetSegmentStates();
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
    if (joyState == JOY_LEFT || joyState == JOY_RIGHT) {
      segmentStates[activeSegment] = !segmentStates[activeSegment];
    }
    swState = getButtonState();
    if (swState && !swPressed) {
      swPressed = true;
    }
    if (!swState && swPressed) {
      systemState = 1;
      swPressed = false;
    }
  }
}

byte getButtonState() {
  swReading = digitalRead(pinSW);
  if (swReading != swLastReading) {
    swLastReading = swReading;
    swDebounceTime = millis();
  }
  if (millis() - swDebounceTime > swDebounceDelay) {
    return !swReading;
  }
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
    activeSegment = getNextSegment(0, 1);
  }
  if (!joyMoved && yValue > maxXThreshold) {
    joyMoved = true;
    return JOY_DOWN;
    activeSegment = getNextSegment(0, -1);
  }
  return NONE;
}

int getNextSegment(int xOffset, int yOffset) {
  if (xOffset != 0) {
    segmentNeighborIndex = xOffset == -1 ? 0 : 1;
  }
  if (yOffset != 0) {
    segmentNeighborIndex = yOffset == -1 ? 3 : 2;
  }
  if (segmentNeighbors[activeSegment][segmentNeighborIndex] != -1) {
    return segmentNeighbors[activeSegment][segmentNeighborIndex];
  }
  return activeSegment;
}

void resetSegmentStates() {
  for (int i = 0; i < segSize; ++i) {
    segmentStates[i] = LOW;
  }
  activeSegment = segSize - 1;
}

void writeOutput() {
  for (int i = 0; i < segSize; ++i) {
    if (systemState == 2) {
      digitalWrite(segments[i], segmentStates[i]);
      continue;
    }
    if (i != activeSegment) {
      digitalWrite(segments[i], segmentStates[i]);
    } else {
      if (millis() - activeSegmentTime > activeSegmentDelay) {
        activeSegmentState = !activeSegmentState;
        activeSegmentTime = millis();
      }
      digitalWrite(segments[i], activeSegmentState);
    }
  }
}