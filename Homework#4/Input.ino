bool joyPressed() {
  static const int debounceDelay = 20;
  static unsigned long lastDebounceTime = 0;
  static byte reading, lastReading = LOW;

  reading = digitalRead(swPin);
  if (reading != lastReading) {
    lastDebounceTime = millis();
    lastReading = reading;
  }
  if (millis() - lastDebounceTime > debounceDelay) {
    return !reading;
  }
}

JoyMovements getJoyMovement() {
  static bool joyMoved = false;
  static int xValue = 0;
  static int yValue = 0;
  const int thresholdOffset = 50;

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  if (xValue > MIN_X_THRESHOLD + thresholdOffset && xValue < MAX_X_THRESHOLD - thresholdOffset) {
    if (yValue > MIN_Y_THRESHOLD + thresholdOffset && yValue < MAX_Y_THRESHOLD - thresholdOffset) {
      joyMoved = false;
    }
  }

  if (!joyMoved && xValue < MIN_X_THRESHOLD) {
    joyMoved = true;
    return JOY_LEFT;
  }
  if (!joyMoved && xValue > MAX_X_THRESHOLD) {
    joyMoved = true;
    return JOY_RIGHT;
  }
  if (!joyMoved && yValue < MIN_Y_THRESHOLD) {
    joyMoved = true;
    return JOY_UP;
  }
  if (!joyMoved && yValue > MAX_Y_THRESHOLD) {
    joyMoved = true;
    return JOY_DOWN;
  }
  return NONE;
}