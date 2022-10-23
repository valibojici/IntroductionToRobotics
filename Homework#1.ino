const int redLedAnalogInPin = A0;
const int greenLedAnalogInPin = A1;
const int blueLedAnalogInPin = A2;
const int redLedPin = 11;
const int greenLedPin = 10;
const int blueLedPin = 9;

const int minAnalogInVal = 0;
const int maxAnalogInVal = 1023;
const int minAnalogOutVal = 0;
const int maxAnalogOutVal = 255;

int redLedInputVal;
int greenLedInputVal;
int blueLedInputVal;

void setup() {
  pinMode(redLedAnalogInPin, INPUT);
  pinMode(greenLedAnalogInPin, INPUT);
  pinMode(blueLedAnalogInPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);
}
void loop() {
  redLedInputVal = analogRead(redLedAnalogInPin);
  greenLedInputVal = analogRead(greenLedAnalogInPin);
  blueLedInputVal = analogRead(blueLedAnalogInPin);

  redLedInputVal = map(redLedInputVal, minAnalogInVal, maxAnalogInVal, minAnalogOutVal, maxAnalogOutVal);
  greenLedInputVal = map(greenLedInputVal, minAnalogInVal, maxAnalogInVal, minAnalogOutVal, maxAnalogOutVal);
  blueLedInputVal = map(blueLedInputVal, minAnalogInVal, maxAnalogInVal, minAnalogOutVal, maxAnalogOutVal);

  setLedColor(redLedInputVal, greenLedInputVal, blueLedInputVal);
}

void setLedColor(int red, int green, int blue){
  analogWrite(redLedPin, red);
  analogWrite(greenLedPin, green);
  analogWrite(blueLedPin, blue);
}