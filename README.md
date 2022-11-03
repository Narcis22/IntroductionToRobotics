# Introduction to Robotics (2022 - 2023)

#### Necula Narcis
Introduction to Robotics laboratory homeworks, 3rd year at the Faculty of Matematics and Computer Science, section Computer Science at the University of Bucharest. This will be composed by weekly homeworks who would include code, pictures and information about the progress made (in code and phisical progress to the Arduino project).


### 📗Homework 0️⃣
<br>

 > <i>Install Arduino and create this repository.☝️</i> <br>

<details>
<summary><i><b>Setup photos</b></i></summary>
<br> 
 
![alt text](https://i.imgur.com/OvHTXBm.png)
 
<br>
</details>

### 📗Homework 1️⃣
<br>

 > RGB LED
 
<i><b>Task:</b></i> Use a separate potentiometer in controlling each of the color of the RGB led (Red, Green and Blue). The control must be done with digital electronics (aka you must read the value of the potentiometer with Arduino, and write a mapped value to each of the pins connected to the led).

<details>
<summary><i><b>Code</b></i></summary>

```
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
```

</details>

<details>
 <summary><i><b>Setup photos</b></i></summary>
 </br></br>
 
![Robotics_Homework_1_setup_2](https://user-images.githubusercontent.com/62501946/197394485-7ab89801-adc1-4e15-951d-9787bf98c7f6.jpg)

![Robotics_Homework_1_setup_1](https://user-images.githubusercontent.com/62501946/197394483-a79f9cf6-b6f3-4325-b838-ec25472c7c68.jpg)

</details>

<i><b>Video with working model:</b></i>
[Go to youtube to see video.](https://www.youtube.com/watch?v=gRUL80wEv6I&ab_channel=NarcisNecula)


### 📗Homework 2️⃣
<br>

 > Crosswalk - 🚦🚸
 
 <i><b>Task:</b></i> Create a crosswalk system which acts like a regular one after pressing the pedestrian button. So, after the button is pressed, a timer starts and after eight seconds the lights start turning yellow and then red for the vehicles and green for pedestrians accompanied by a buzzer. After ten seconds of green light for the pedestrians, the light starts blinking and the buzzing intensifies for five seconds after which it turns back red and the traffic light turns green and the whole process can be started again.


<details>
<summary><i><b>Code</b></i></summary>

```
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
```

</details>

<details>
 
 <summary> <i><b>Setup photos</b></i> </summary>
</br></br>

![Crosswalk_unlit](https://user-images.githubusercontent.com/62501946/198887521-6087d977-56da-496f-adb0-36144fec0ba5.jpg)

![Crosswalk_lit](https://user-images.githubusercontent.com/62501946/198887522-a0d7b4f9-b529-4397-8370-edde4f75c010.jpg)

</details>

<i><b>Video with working model: </b></i>
[Go to youtube to see video.](https://youtu.be/yUhXWoTgpPg)
 
### 📕Homework 3️⃣
<br>

 > TBD
 
![Homework_not_found](https://user-images.githubusercontent.com/62501946/198888759-e419866f-99fa-4ebb-8b61-47aef8e703dd.png)


