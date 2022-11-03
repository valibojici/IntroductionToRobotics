const int carRedLedPin = 13;
const int carYellowLedPin = 12;
const int carGreenLedPin = 11;
const int peopleRedLedPin = 7;
const int peopleGreenLedPin = 6;
const int buttonPin = 2;
const int buzzerPin = 10;

unsigned long lastStateStartTime = 0;
byte systemState = 1;
byte carRedLedState;
byte carYellowLedState;
byte carGreenLedState;
byte peopleGreenLedState;
byte peopleRedLedState;
unsigned long peopleBlinkingGreenStart;
unsigned int peopleBlinkingGreenDelay = 200;
byte buzzerState = LOW;
unsigned int buzzerFrequency;
unsigned int normalBuzzerFrequency = 220;
unsigned int fastBuzzerFrequency = 300;
unsigned long buzzerStart;
unsigned int normalBuzzerDelay = 600;
unsigned int fastBuzzerDelay = 300;
byte reading;
byte buttonState = LOW;
byte lastReading = LOW;
bool pressedButton = false;
unsigned long lastDebounceTime;
unsigned int debounceDelay = 50;
unsigned int stateDurations[] = { 0, 3000, 8000, 4000 };
unsigned int buttonPressWaitingTime = 8000;

void setup() {
  pinMode(carRedLedPin, OUTPUT);
  pinMode(carYellowLedPin, OUTPUT);
  pinMode(carGreenLedPin, OUTPUT);
  pinMode(peopleGreenLedPin, OUTPUT);
  pinMode(peopleRedLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  executeState();
  writeOutput();
}

void executeState() {
  setBuzzerState();
  setLEDStates();

  if (!currentStateHasEnded()) return;

  if (systemState == 1) {
    checkButtonPress();
    if (pressedButton && millis() - lastStateStartTime > buttonPressWaitingTime) {
      systemState = 2;
      pressedButton = false;
      lastStateStartTime = millis();
    }
  } else if (systemState == 2) {
    systemState = 3;
    lastStateStartTime = millis();
    buzzerStart = millis();
    buzzerState = HIGH;
  } else if (systemState == 3) {
    systemState = 4;
    lastStateStartTime = millis();
  } else if (systemState == 4) {
    systemState = 1;
    lastStateStartTime = millis();
    peopleBlinkingGreenStart = millis();
  }
}

bool currentStateHasEnded() {
  if (millis() - lastStateStartTime > stateDurations[systemState - 1]) {
    return true;
  }
  return false;
}

void checkButtonPress() {
  reading = digitalRead(buttonPin);
  if (reading != lastReading) {
    lastDebounceTime = millis();
    lastReading = reading;
  }

  if (!pressedButton && millis() - lastDebounceTime > debounceDelay) {
    if (!reading) {
      pressedButton = true;
      lastStateStartTime = millis();
    }
  }
}

void setBuzzerState() {
  if (systemState == 3) {
    buzzerFrequency = normalBuzzerFrequency;
    if (millis() - buzzerStart > normalBuzzerDelay) {
      buzzerState = !buzzerState;
      buzzerStart = millis();
    }
    return;
  }
  if (systemState == 4) {
    buzzerFrequency = fastBuzzerFrequency;
    if (millis() - buzzerStart > fastBuzzerDelay) {
      buzzerState = !buzzerState;
      buzzerStart = millis();
    }
    return;
  }
  buzzerState = LOW;
}

void setLEDStates() {
  if (systemState == 1) {
    carGreenLedState = HIGH;
    carYellowLedState = LOW;
    carRedLedState = LOW;
    peopleGreenLedState = LOW;
    peopleRedLedState = HIGH;
  } else if (systemState == 2) {
    carGreenLedState = LOW;
    carYellowLedState = HIGH;
  } else if (systemState == 3) {
    carRedLedState = HIGH;
    carYellowLedState = LOW;
    peopleGreenLedState = HIGH;
    peopleRedLedState = LOW;
  } else if (systemState == 4) {
    if (millis() - peopleBlinkingGreenStart > peopleBlinkingGreenDelay) {
      peopleGreenLedState = !peopleGreenLedState;
      peopleBlinkingGreenStart = millis();
    }
  }
}

void writeOutput() {
  if (buzzerState) {
    tone(buzzerPin, buzzerFrequency);
  } else {
    noTone(buzzerPin);
  }

  digitalWrite(carRedLedPin, carRedLedState);
  digitalWrite(carYellowLedPin, carYellowLedState);
  digitalWrite(carGreenLedPin, carGreenLedState);
  digitalWrite(peopleGreenLedPin, peopleGreenLedState);
  digitalWrite(peopleRedLedPin, peopleRedLedState);
}