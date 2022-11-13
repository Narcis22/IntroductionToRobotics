const int dataPin = 12;
const int latchPin = 11;
const int clockPin = 10;

// display pins and ports
const int segD1 = 7;
const int segD2 = 6;
const int segD3 = 5;
const int segD4 = 4;

const int displayCount = 4;
int displaySegments[displayCount] = { segD1, segD2, segD3, segD4 };

int displayDigits[displayCount] = { 0, 0, 0, 0};

const int encodingNumber = 16;

// setting up codes for the HEX digits to display
int digitArray[encodingNumber] = {
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

// Joystick setup

const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;

bool swState = LOW;
bool lastSwState = LOW;
byte switchState = HIGH;

int xValue = 0;
int yValue = 0;

bool joyMovedX = false;
bool joyMovedY = false;

int minThreshold = 400;
int maxThreshold = 600;

bool dpState = false;
bool selectedState = false;
int currentDisplay = 0;
unsigned long dpBlink = 0;

const int firstBlinkInterval = 400;
const int secondBlinkInterval = 800;

unsigned long lastDebounceTime = 0;
unsigned int debounceDelay = 50;
unsigned long pressTime = 0;
unsigned long releaseTime = 0;
const int longPressDuration = 1000;
const int shortPressDuration = 50;

void setup() {
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  for (int i = 0; i < displayCount; i++) {
    pinMode(displaySegments[i], OUTPUT);
    digitalWrite(displaySegments[i], LOW);
  }
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  
  changeDisplay();
  
  swState = digitalRead(pinSW);
  buttonChangeState();

  writeNumber(currentDisplay, selectedState);
}

// Move on the Y axis and change the selected display
void changeDisplay() {
  if (yValue < minThreshold && !joyMovedY && !selectedState) {
    if (currentDisplay < 3) {
      ++currentDisplay;
    }
    joyMovedY = true;
  }
  if (yValue > maxThreshold && !joyMovedY && !selectedState) {
    if (currentDisplay > 0) {
      --currentDisplay;
    }
    joyMovedY = true;
  }
  if (yValue > minThreshold && yValue < maxThreshold && !selectedState) {
    joyMovedY = false;
  }
}

void buttonChangeState() {
  if (swState != lastSwState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (swState != switchState) {
      switchState = swState;

      if (switchState == LOW) {
        pressTime = millis();
      } else {
        releaseTime = millis();
      }

      long pressDuration = releaseTime - pressTime;
      if (pressDuration > longPressDuration && selectedState == 0) {
        resetDisplays();
      } else if (pressDuration > shortPressDuration) {
        selectedState = !selectedState;
      }        
    }
  }

  if (selectedState) {
    if (xValue > maxThreshold && !joyMovedX){
      if (displayDigits[currentDisplay] < 15) {
        ++displayDigits[currentDisplay];
      }
      joyMovedX = true;
    }
    if (xValue < minThreshold && !joyMovedX) {
      if (displayDigits[currentDisplay] > 0) {
        --displayDigits[currentDisplay];
      }
      joyMovedX = true;
    }
    if (xValue >= minThreshold && xValue <= maxThreshold){
      joyMovedX = false;
    }
  }

  lastSwState = swState;
}

void writeNumber(int currentDisplay, int selectedState) {
  for (int i = 0; i < displayCount; ++i) {
    int digitToWrite = 0;
    if (i == currentDisplay) {
      if (selectedState) {
        digitToWrite = digitArray[displayDigits[currentDisplay]] + 1;
      } else {
        digitToWrite = digitArray[displayDigits[currentDisplay]];

        if (millis() - dpBlink > firstBlinkInterval) {
          digitToWrite = digitArray[displayDigits[currentDisplay]] + 1;
        }
        if (millis() - dpBlink > secondBlinkInterval) {
          digitToWrite = digitArray[displayDigits[currentDisplay]];
          dpBlink = millis();
        }        
      }
    } else {
        digitToWrite = digitArray[displayDigits[i]];
    }

    writeReg(digitToWrite);
    showDigit(i);
    delay(5);
  }
}

// function to activate and deactivate displays
void showDigit(int displayNo) {
  for (int i = 0; i < displayCount; ++i) {
    digitalWrite(displaySegments[i], HIGH);
  }

  digitalWrite(displaySegments[displayNo], LOW);
}


// function to activate and deactivate displays
void writeReg(int digitToWrite) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, digitToWrite);
  digitalWrite(latchPin, HIGH);
}

// reset after long press
void resetDisplays() {
  for (int i = 0; i < displayCount; ++i) {
    displayDigits[i] = 0;
  }

  currentDisplay = 0;
}




















