// Joystick pin declaration - in connection to ports
const int pinSW = 2;
const int pinX = A0; 
const int pinY = A1;

// joystick potentiometers values 
int xValue = 0;
int yValue = 0;
const int minThreshold = 400;
const int maxThreshold = 600;

// joystick switch (button) values
byte stateSW = LOW;
byte lastSwState = HIGH;
byte switchState = HIGH;

byte middleSegmentYAxis_State2 = LOW;
byte middleSegmentYAxis_State1 = LOW;
byte middleSegmentXAxis = LOW;

// display pins and states for each led
const int pinA = 4;
const int pinB = 5;
const int pinC = 6;
const int pinD = 7;
const int pinE = 8;
const int pinF = 9;
const int pinG = 10;
const int pinDP = 11;

byte stateLedA = LOW;
byte stateLedB = LOW;
byte stateLedC = LOW;
byte stateLedD = LOW;
byte stateLedE = LOW;
byte stateLedF = LOW;
byte stateLedG = LOW;
byte stateLedDP = LOW;

const int noSegments = 8;
int segments[noSegments] = { pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP };

// blinking current state
int currentBlinkingPin = 11;
byte blinkingState = LOW;
byte blinkStopped = LOW;

unsigned long lastDebounceTime = 0;
const int debounceDelay = 50;
const int blinkInterval = 400;

unsigned long lastBlinkTime = 0;
unsigned long pressTime;
unsigned long releaseTime;
unsigned long noPresses = 0;

const int longPressTreshold = 1000;
const int shortPressTreshold = 50;

int currentState = 1; // 1 is the "cursor" state and 2 is the ON/OFF state for the led segments

void setup() {
  for (int i = 0; i < noSegments; i++)
    pinMode(segments[i], OUTPUT);

  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
}

// controll for which leds to be on and off
void displayControl() {
  if (currentBlinkingPin != pinA)
    digitalWrite(pinA, stateLedA);
  if (currentBlinkingPin != pinB)
    digitalWrite(pinB, stateLedB);
  if (currentBlinkingPin != pinC)
    digitalWrite(pinC, stateLedC);
  if (currentBlinkingPin != pinD)
    digitalWrite(pinD, stateLedD);
  if (currentBlinkingPin != pinE)
    digitalWrite(pinE, stateLedE);
  if (currentBlinkingPin != pinF)
    digitalWrite(pinF, stateLedF);
  if (currentBlinkingPin != pinG)
    digitalWrite(pinG, stateLedG);
  if (currentBlinkingPin != pinDP)
    digitalWrite(pinDP, stateLedDP);
    
  digitalWrite(currentBlinkingPin, blinkingState);
}

// reset to start form
void resetLeds() {
  stateLedA = LOW;
  stateLedB = LOW;
  stateLedC = LOW;
  stateLedD = LOW;
  stateLedE = LOW;
  stateLedF = LOW;
  stateLedG = LOW;
  stateLedDP = LOW;
  
  currentBlinkingPin = pinDP;
}

// move currect segment (move hover for the segment to whick i can apply changes)
void movementForTheCursor() {
  if (yValue < minThreshold && middleSegmentYAxis_State1 == LOW && currentState == 1) {

    if (currentBlinkingPin == pinA) {
      currentBlinkingPin = pinF;
      Serial.println("Go left from A to F");
    }
    else if (currentBlinkingPin == pinB) {
      currentBlinkingPin = pinF;
      Serial.println("Go left from B to F");
    }
    else if (currentBlinkingPin == pinC) {
      currentBlinkingPin = pinE;
      Serial.println("Go left from C to E");
    }
    else if (currentBlinkingPin == pinDP) {
      currentBlinkingPin = pinC;
      Serial.println("Go left from DP to C");
    }
    else if (currentBlinkingPin == pinD) {
      currentBlinkingPin = pinE;
      Serial.println("Go left from D to E");
    }
    
    middleSegmentYAxis_State1 = HIGH;
  }
  else if (yValue > maxThreshold && middleSegmentYAxis_State1 == LOW && currentState == 1) {

    if (currentBlinkingPin == pinA || currentBlinkingPin == pinF) {
      currentBlinkingPin = pinB;
      Serial.println("Go right from A to B");
    }
    else if (currentBlinkingPin == pinF) {
      currentBlinkingPin = pinB;
      Serial.println("Go right from F to B");
    }
    else if (currentBlinkingPin == pinC) {
      currentBlinkingPin = pinDP;
      Serial.println("Go right from C to DP");
    }
    else if (currentBlinkingPin == pinE) {
      currentBlinkingPin = pinC;
      Serial.println("Go right from E to C");
    }
    else if (currentBlinkingPin == pinD) {
      currentBlinkingPin = pinC;
      Serial.println("Go right from D to C");
    }

    middleSegmentYAxis_State1 = HIGH; 
  }
  else if (middleSegmentYAxis_State1 == HIGH && yValue < maxThreshold && yValue > minThreshold && currentState == 1)
    middleSegmentYAxis_State1 = LOW;

  if (xValue < minThreshold && middleSegmentXAxis == LOW && currentState == 1) {

    if (currentBlinkingPin == pinG) {
      currentBlinkingPin = pinD;
      Serial.println("Go down from G to D");
    }
    else if (currentBlinkingPin == pinA) {
      currentBlinkingPin = pinG;
      Serial.println("Go down from A to G");
    }
    else if (currentBlinkingPin == pinB) {
      currentBlinkingPin = pinG;
      Serial.println("Go down from B to G");
    }
    else if (currentBlinkingPin == pinF) {
      currentBlinkingPin = pinG;
      Serial.println("Go down from F to G");
    }
    else if (currentBlinkingPin == pinC) {
      currentBlinkingPin = pinD;
      Serial.println("Go down from C to D");
    }
    else if (currentBlinkingPin == pinE) {
      currentBlinkingPin = pinD;
      Serial.println("Go down from E to D");
    }

    middleSegmentXAxis = HIGH;
  }
  else if (xValue > maxThreshold && middleSegmentXAxis == LOW && currentState == 1) {

    if (currentBlinkingPin == pinG) {
      currentBlinkingPin = pinA;
      Serial.println("Go up from G to A");
    }
    else if (currentBlinkingPin == pinD) {
      currentBlinkingPin = pinG;
      Serial.println("Go up from D to G");
    }
    else if (currentBlinkingPin == pinE) {
      currentBlinkingPin = pinG;
      Serial.println("Go up from E to G");
    }
    else if (currentBlinkingPin == pinC) {
      currentBlinkingPin = pinG;
      Serial.println("Go up from C to G");
    }
    else if (currentBlinkingPin == pinB) {
      currentBlinkingPin = pinA;
      Serial.println("Go up from B to A");
    }
    else if (currentBlinkingPin == pinF) {
      currentBlinkingPin = pinA;
      Serial.println("Go up from F to A");
    }

    middleSegmentXAxis = HIGH;
  }
  else if (middleSegmentXAxis == HIGH && xValue < maxThreshold && xValue > minThreshold && currentState == 1)
    middleSegmentXAxis = LOW;
}

// choose what to do with the selected pin (on or off)
void changeSelectedPinState() {
  if (yValue < minThreshold && middleSegmentYAxis_State2 == LOW && currentState == 2) {

    blinkingState = LOW;
    if (currentBlinkingPin == pinA) {
      stateLedA = LOW;
      Serial.println("Turn off A");
    }
    else if (currentBlinkingPin == pinB) {
      stateLedB = LOW;
      Serial.println("Turn off B");
    }
    else if (currentBlinkingPin == pinC) {
      stateLedC = LOW;
      Serial.println("Turn off C");
    }
    else if (currentBlinkingPin == pinD) {
      stateLedD = LOW;
      Serial.println("Turn off D");
    }
    else if (currentBlinkingPin == pinE) {
      stateLedE = LOW;
      Serial.println("Turn off E");
    }
    else if (currentBlinkingPin == pinF) {
      stateLedF = LOW;
      Serial.println("Turn off F");
    }
    else if (currentBlinkingPin == pinG) {
      stateLedG = LOW;
      Serial.println("Turn off G");
    }
    else if (currentBlinkingPin == pinDP) {
      stateLedDP = LOW;
      Serial.println("Turn off DP");
    }

    middleSegmentYAxis_State2 = HIGH;
  }
  else if (yValue > maxThreshold && middleSegmentYAxis_State2 == LOW && currentState == 2) {

    blinkingState = HIGH;
    if (currentBlinkingPin == pinA) {
      stateLedA = HIGH;
      Serial.println("Turn on A");
    }
    else if (currentBlinkingPin == pinB) {
      stateLedB = HIGH;
      Serial.println("Turn on B");
    }
    else if (currentBlinkingPin == pinC) {
      stateLedC = HIGH;
      Serial.println("Turn on C");
    }
    else if (currentBlinkingPin == pinD) {
      stateLedD = HIGH;
      Serial.println("Turn on D");
    }
    else if (currentBlinkingPin == pinE) {
      stateLedE = HIGH;
      Serial.println("Turn on E");
    }
    else if (currentBlinkingPin == pinF) {
      stateLedF = HIGH;
      Serial.println("Turn on F");
    }
    else if (currentBlinkingPin == pinG) {
      stateLedG = HIGH;
      Serial.println("Turn on G");
    }
    else if (currentBlinkingPin == pinDP) {
      stateLedDP = HIGH;
      Serial.println("Turn on DP");
    }
    
    middleSegmentYAxis_State2 = HIGH; 
  }
  else if (middleSegmentYAxis_State2 == HIGH && yValue < maxThreshold && yValue > minThreshold && currentState == 2)
    middleSegmentYAxis_State2 = LOW;
}

// Change the state: reset display, go to state 1 or to state 2
void buttonChangeState() {
  if (stateSW != lastSwState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (stateSW != switchState) {
      switchState = stateSW;
      if (switchState == LOW)
        pressTime = millis();
      else
        releaseTime = millis();

      long pressDuration = releaseTime - pressTime;
      if (pressDuration > longPressTreshold && currentState == 1)
        resetLeds();
      else if (pressDuration > shortPressTreshold) {
        noPresses ++;
        if (noPresses % 2 != 0) {
          blinkStopped = HIGH;
          currentState = 2;
        }
        else {
          blinkStopped = LOW;
          currentState = 1;
        }
      }
    }
  }
  
  lastSwState = stateSW;
}

void blink() {
  if (millis() - lastBlinkTime > blinkInterval && blinkStopped == LOW) {
    blinkingState = !blinkingState;
    lastBlinkTime = millis();
  }
}

void loop() {
  displayControl();

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  blink();
  movementForTheCursor();
  changeSelectedPinState();

  stateSW = digitalRead(pinSW);

  buttonChangeState(); 
}
