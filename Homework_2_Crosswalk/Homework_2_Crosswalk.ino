// set the buzzer, button and led pins
const int buttonPin = 2;
const int buzzerPin = 3;
const int pedestrianGreenPin = 4;
const int pedestrianRedPin = 5;
const int vehiclesGreenPin = 6;
const int vehiclesYellowPin = 7;
const int vehiclesRedPin = 8;

// set the timers for each state
const int timerState1 = 8000;
const int timerState2 = 3000;
const int timerState3 = 10000;
const int timerState4 = 5000;

// timings and settings for buzzer and blinkinmg green light for pedestrians
const int buzzerSound = 250;
const int buzzerTime = 250;
const int buzzerFastPacedTime = 150;
const int blinkingTime = 300;
int blinkingState = LOW;

// last timer for blinking green light and buzzer
unsigned long lastBuzz = 0;
unsigned long lastBlink = 0;

int reading = 0;
byte buttonWasPressed = 0;
byte lastButtonState = LOW;
byte currentButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

int currentState = 1;
unsigned long timer = 0;

void setup(){
  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(pedestrianGreenPin, OUTPUT);
  pinMode(pedestrianRedPin, OUTPUT);
  pinMode(vehiclesGreenPin, OUTPUT);
  pinMode(vehiclesYellowPin, OUTPUT);
  pinMode(vehiclesRedPin, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  checkButtonState();
  changeState();

  if (currentState == 1) {
    applyState1();
  } else if (currentState == 2) {
    applyState2();
  } else if (currentState == 3) {
    applyState3();
  } else if (currentState == 4) {
    applyState4();
  }
}

void checkButtonState() {
  reading = digitalRead(buttonPin);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime > debounceDelay) {
    if (reading != currentButtonState) {
      currentButtonState = reading;

      if (currentButtonState == LOW && currentState == 1) {
        if (!buttonWasPressed) {
          buttonWasPressed = 1;
          timer = millis();
        }
      }
    }
  } 
  lastButtonState = reading;
}

void changeState() {
  if (currentState == 1 && buttonWasPressed && millis() - timer > timerState1) {
    buttonWasPressed = 0;
    currentState = 2;
    timer = millis();    
  } else if (currentState == 2 && millis() - timer > timerState2) {
    currentState = 3;
    timer = millis();
  } else if (currentState == 3 && millis() - timer > timerState3) {
    currentState = 4;
    timer = millis();
  } else if (currentState == 4 && millis() - timer > timerState4) {
    currentState = 1;
    timer = millis();
  }
}

void applyState1(){
  // pedestrian lights
  digitalWrite(pedestrianGreenPin, LOW);
  digitalWrite(pedestrianRedPin, HIGH);

  // vehicles lights
  digitalWrite(vehiclesGreenPin, HIGH);
  digitalWrite(vehiclesYellowPin, LOW);
  digitalWrite(vehiclesRedPin, LOW);

  // buzzer tone
  noTone(buzzerPin);
}

void applyState2(){
  // pedestrian lights
  digitalWrite(pedestrianGreenPin, LOW);
  digitalWrite(pedestrianRedPin, HIGH);

  // vehicles lights
  digitalWrite(vehiclesGreenPin, LOW);
  digitalWrite(vehiclesYellowPin, HIGH);
  digitalWrite(vehiclesRedPin, LOW);

  // buzzer tone
  noTone(buzzerPin);
}

void applyState3(){
  // pedestrian lights
  digitalWrite(pedestrianGreenPin, HIGH);
  digitalWrite(pedestrianRedPin, LOW);

  // vehicles lights
  digitalWrite(vehiclesGreenPin, LOW);
  digitalWrite(vehiclesYellowPin, LOW);
  digitalWrite(vehiclesRedPin, HIGH);

  // buzzer tone
  if (millis() - lastBuzz > buzzerTime) {
    noTone(buzzerPin);
  }

  if (millis() - lastBuzz > 2 * buzzerTime){
    tone(buzzerPin, buzzerSound);
    Serial.println("Sound is playing\n");
    lastBuzz = millis();
  }
}

void applyState4(){
  // pedestrian lights
  // blinking green light
  if (millis() - lastBlink > blinkingTime) {
    blinkingState = !blinkingState;
    lastBlink = millis();
  }
  digitalWrite(pedestrianGreenPin, blinkingState);
  digitalWrite(pedestrianRedPin, LOW);

  // vehicles lights
  digitalWrite(vehiclesGreenPin, LOW);
  digitalWrite(vehiclesYellowPin, LOW);
  digitalWrite(vehiclesRedPin, HIGH);

  // buzzer tone
  if (millis() - lastBuzz > buzzerFastPacedTime) {
    noTone(buzzerPin);
  }
  
  if (millis() - lastBuzz > 2 * buzzerFastPacedTime) {
    tone(buzzerPin, buzzerSound);
    Serial.println("Fast sound is playing\n");
    lastBuzz = millis();
  }
}





















































