const int redLedPin = 11;
const int greenLedPin = 10;
const int blueLedPin = 9;

const int potForBluePin = A0;
const int potForGreenPin = A1;
const int potForRedPin = A2;

int potForBlueValue = 0;
int potForGreenValue = 0;
int potForRedValue = 0;

const int minLedValue = 0;
const int maxLedValue = 255;
const int minPotValue = 0;
const int maxPotValue = 1023;

void setup() {
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);

  pinMode(potForBluePin, INPUT);
  pinMode(potForGreenPin, INPUT);
  pinMode(potForRedPin, INPUT);

  Serial.begin(9600);
}

void loop() {
  potForBlueValue = analogRead(potForBluePin);
  potForGreenValue = analogRead(potForGreenPin);
  potForRedValue = analogRead(potForRedPin);

  setColor(potForRedValue, potForGreenValue, potForBlueValue);
}

void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redLedPin, map(redValue, minPotValue, maxPotValue, minLedValue, maxLedValue));
  analogWrite(greenLedPin, map(greenValue, minPotValue, maxPotValue, minLedValue, maxLedValue));
  analogWrite(blueLedPin, map(blueValue, minPotValue, maxPotValue, minLedValue, maxLedValue));
}