#include <LiquidCrystal.h>
#include "LedControl.h"
#include <EEPROM.h>
#include "pitches.h"

const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;

const byte RS = 9;
const byte enable = 8;
const byte d4 = 7;
const byte d5 = 3;
const byte d6 = 5;
const byte d7 = 4;
const byte lcdBrightnessControlPin = 6;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

// joystick setup
const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;

byte swState = HIGH;
byte lastSwState = HIGH;

int xValue = 0;
int yValue = 0;

bool xJoystickState = false;
bool yJoystickState = false;
const int minThreshold = 400;
const int maxThreshold = 600;

byte reading = 1;
unsigned long lastDebounceTime = 0;
const unsigned int debounceDelay = 50;
unsigned long startGreetingTime = 0;
const unsigned int greetingDuration = 5000;
const unsigned int gameOverDuration = 5000;
unsigned long startGameOverSequence = 0;

bool hasChosenSaveOption = false;

const String greetingMessage[2] = {
  "   Snake",
  "     by Narcis"
};

byte indexText = 1;
const String aboutSection[] = {
  "Game: Snake",
  "Dev: Necula Narcis          ",
  "GitHub: https://github.com/Narcis22          ",
  "Intro to Robotics course, University of Bucharest.         ",
  "Go back"
};

byte menuState;
byte mainMenuView;
byte infoMenuView;
byte settingsMenuView;
byte htpMenuView;
byte leaderboardView;

bool saveName;
bool hasNameSet = false;
String playerName = "AAAAA";

byte yJoystickPosition = 0;
byte difficulty = 0;
bool activateSound;
bool soundHasPlayed = false;

byte lcdBrightness;
byte matrixBrightness;

byte brightnessItem; // matrix or lcd

byte noExistentHighscores;
String leaderboardNames[5] = {};
int leaderboardScores[5] = {};

bool scoreMarked = false;
byte insertPosition;

bool playing;
byte snakePositionY;
byte snakePositionX;
byte applePositionY;
byte applePositionX;
long score;
byte livesLeft;

// blinking interval
const int blinkTimeOn = 400;
const int blinkTimeOff = 800;
unsigned long lastBlink = 0;

int buzzer = A2;

// Custom characters
byte arrowUp[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01010,
  0b10001,
  0b00000,
  0b00000,
  0b00000
};

byte arrowDown[8] = {
	0b00000,
	0b00000,
	0b00000,
	0b10001,
	0b01010,
	0b00100,
	0b00000,
	0b00000
};

byte currentLetterArrow[8] = {
  0b00100,
  0b01010,
  0b10001,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte saveArrow[8] = {
  0b00000,
  0b00100,
  0b00010,
  0b00001,
  0b00010,
  0b00100,
  0b00000,
  0b00000
};

byte speaker[8] = {
  0b00000,
  0b00010,
  0b00110,
  0b01110,
  0b01110,
  0b00110,
  0b00010,
  0b00000
};

byte soundOff[8] = {
  0b00000,
  0b00000,
  0b10010,
  0b01100,
  0b01100,
  0b10010,
  0b00000,
  0b00000
};

byte soundOn[8] = {
  0b11000,
  0b00100,
  0b10010,
  0b01010,
  0b01010,
  0b10010,
  0b00100,
  0b11000
};

byte brightnessBar[8] = {
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110,
  0b01110
};

byte brightnessPlaceholder[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b01110
};

byte heart[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000
};

// Matrix custom images
const uint8_t DisplayImages[][8] = {
{
  0b11111111,
  0b10000001,
  0b10111101,
  0b10100101,
  0b10101101,
  0b10100001,
  0b10111111,
  0b10000000
},{
  0b10100101,
  0b01000010,
  0b10100101,
  0b00000000,
  0b01111110,
  0b00001100,
  0b00001100,
  0b00000000
}};
const int DisplayImagesLen = sizeof(DisplayImages) / 8;

int whatToDisplay = 0;
int whatIsDisplayed = 0;

void setup() {
  
  Serial.begin(9600);
  
  lcd.begin(16, 2);
  pinMode(pinSW, INPUT_PULLUP);
  
  pinMode(lcdBrightnessControlPin, OUTPUT);

  // initializeEEPROM(); // run only once to insert default values in eeprom / reset all values in eeprom
  setDataFromEEPROM();
  getLeaderboardFromEEPROM();
  
  analogWrite(lcdBrightnessControlPin, lcdBrightness);
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  
  printImageOnMatrix(DisplayImages[0]);

  startGreetingTime = millis();
  menuState = 0;
  mainMenuView = 0;

  lcd.createChar(6, arrowUp);
  lcd.createChar(7, arrowDown);
}

void loop() {
  if (whatToDisplay != whatIsDisplayed) {
    printImageOnMatrix(DisplayImages[whatToDisplay]);
    whatIsDisplayed = whatToDisplay;
  }

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  reading = digitalRead(pinSW);
  if (reading != lastSwState) {
    lastDebounceTime = millis();
  }

  if (millis() - startGreetingTime < greetingDuration) {
    lcd.setCursor(0, 0);
    lcd.print(greetingMessage[0]);

    lcd.setCursor(0, 1);
    lcd.print(greetingMessage[1]);
  }
  else {
    if (millis() - lastDebounceTime > debounceDelay) {
      if (reading != swState) {    
        swState = reading;
      }
    }
    lastSwState = reading;

    if (menuState == 0) {
      whatToDisplay = 0;
      
      delay(30);
      
      switch (mainMenuView) {
        case 0: 
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> Start Game");

          lcd.setCursor(1, 1);
          lcd.print("Leaderboard");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);

          delay(50);

          if (swState == LOW){
            swState = HIGH;
            lastSwState = HIGH;            
            playing = true;
            
            snakePositionY = random(matrixSize);
            snakePositionX = random(matrixSize);
            applePositionY = random(matrixSize);
            applePositionX = random(matrixSize);

            score = 0;
            livesLeft = 3;
            soundHasPlayed = false;

            lcd.createChar(0, heart);
            lcd.clear();
            lcd.setCursor(1, 0);
            lcd.print("Start game");
            
            delay(50);
            menuState = 1;
          }

          mainMenuView = setMenuState(mainMenuView, 1, mainMenuView); 
          break;
        
        case 1: // Leaderboard
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Start Game");
          lcd.setCursor(0, 1);
          lcd.print("> Leaderboard");
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            leaderboardView = 0;
            menuState = 2;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuView = setMenuState(mainMenuView, 2, 0);
          break;

        case 2: // Settings
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Leaderboard");
          lcd.setCursor(0, 1);
          lcd.print("> Settings");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            settingsMenuView = 0;
            menuState = 3;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuView = setMenuState(mainMenuView, 4, 3);
          break;

        case 3: // Leaderboard
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> Leaderboard");

          lcd.setCursor(1, 1);
          lcd.print("Settings");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            leaderboardView = 0;
            menuState = 2;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuView = setMenuState(mainMenuView, 2, 0);
          break;

        case 4: // About
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Settings");

          lcd.setCursor(0, 1);
          lcd.print("> About");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            infoMenuView = 0;
            menuState = 4;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuView = setMenuState(mainMenuView, 6, 5);
          break;

        case 5: // Settings
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> Settings");

          lcd.setCursor(1, 1);
          lcd.print("About");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            settingsMenuView = 0;
            menuState = 3;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuView = setMenuState(mainMenuView, 4, 3);
          break;
        
        case 6: // How To Play
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("About");

          lcd.setCursor(0, 1);
          lcd.print("> How to play");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          delay(50);

          if (swState == LOW){
            htpMenuView = 0;
            menuState = 5;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuView = setMenuState(mainMenuView, mainMenuView, 7);
          break;
        
        case 7: // About
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> About");

          lcd.setCursor(1, 1);
          lcd.print("How to play");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          delay(50);

          if (swState == LOW){
            infoMenuView = 0;
            menuState = 4;
            swState = HIGH;
            lastSwState = HIGH;
          }

          mainMenuView = setMenuState(mainMenuView, 6, 5);
          break;
      } 
    } else if (menuState == 1) {
      if (playing) {
        for (int row = 0; row < matrixSize; row++) {
          for (int col = 0; col < matrixSize; col++) {
            lc.setLed(0, row, col, false); // turns off LED at col, row
          }
        }

        applesBehaviour();
        displayInGameStats();

        lc.setLed(0, snakePositionX, snakePositionY, true);
        snakeBehaviour();

        if (score == 10) {
          playing = false;
          if (activateSound && !soundHasPlayed) {
            soundHasPlayed = true;
            gameOverSound();          
          }
          resetMatrix();
          startGameOverSequence = millis();
        }

      } else{ 

        whatToDisplay = 1;        
        lcd.createChar(0, currentLetterArrow);
        lcd.createChar(1, saveArrow);
                
        showGameEndedSequence();

        if (scoreMarked) {
          if (insertPosition >= 5) {
              // didn't make the cut
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print("You didn't make");
              
              lcd.setCursor(0,1);
              lcd.print("the cut my G :P");

            } else {
              if (insertPosition == noExistentHighscores) {
                // Leader board but not top 1(last in the leaderboard)
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("  You made it!");

                lcd.setCursor(0, 1);
                lcd.print("In the Top 5! gg");

              } else {
                if (insertPosition == 0) {
                  // Top 1
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("  You made it!");

                  lcd.setCursor(0, 1);
                  lcd.print(" Highest score!"); 

                } else {
                  lcd.clear();
                  lcd.setCursor(0, 0);
                  lcd.print("  You made it!");

                  lcd.setCursor(0, 1);
                  lcd.print("In the Top 5! gg"); 
                }
              }
            }

            if (swState == LOW) { 
              menuState = 0;
              swState = HIGH;
              lastSwState = HIGH;
              scoreMarked = false;
            }
          }
          delay(50);
        }
    
    } else if (menuState == 2) {  // view leaderboard Top 5
      if (noExistentHighscores == 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("  LEADERBOARD");

        lcd.setCursor(0, 1);
        lcd.print("Nobody Worthy!");
        delay(50);

      } else {
        switch (leaderboardView) {
          case 0:
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("  LEADERBOARD");

            lcd.setCursor(0, 1);
            lcd.print("1." + leaderboardNames[0]);

            lcd.setCursor(11, 1);
            lcd.print(leaderboardScores[0]);
            delay(50);

            leaderboardView = setMenuState(leaderboardView, 1, leaderboardView);
            break;
          
          case 1:
            lcd.clear();

            lcd.setCursor(0, 0);
            lcd.print("1." + leaderboardNames[0]);

            lcd.setCursor(11, 0);
            lcd.print(leaderboardScores[0]);

            lcd.setCursor(0, 1);
            if (noExistentHighscores > 1) {
              lcd.print("2." + leaderboardNames[1]);

              lcd.setCursor(11, 1);
              lcd.print(leaderboardScores[1]);
            } else {
              lcd.print("2.");
            }
            delay(50);

            leaderboardView = setMenuState(leaderboardView, 2, 0);
            break;

          case 2:
            lcd.clear();

            lcd.setCursor(0, 0);
            if (noExistentHighscores > 1) {
              lcd.print("2." + leaderboardNames[1]);

              lcd.setCursor(11, 0);
              lcd.print(leaderboardScores[1]);
            } else {
              lcd.print("2.");
            }

            lcd.setCursor(0, 1);
            if (noExistentHighscores > 2) {
              lcd.print("3." + leaderboardNames[2]);

              lcd.setCursor(11, 1);
              lcd.print(leaderboardScores[2]);
            } else {
              lcd.print("3.");
            }
            delay(50);

            leaderboardView = setMenuState(leaderboardView, 3, 1);
            break;

          case 3:
            lcd.clear();

            lcd.setCursor(0, 0);
            if (noExistentHighscores > 2) {
              lcd.print("3." + leaderboardNames[2]);

              lcd.setCursor(11, 0);
              lcd.print(leaderboardScores[2]);

            } else {
              lcd.print("3.");
            }

            lcd.setCursor(0, 1);
            if (noExistentHighscores > 3) {
              lcd.print("4." + leaderboardNames[3]);

              lcd.setCursor(11, 1);
              lcd.print(leaderboardScores[3]);

            } else {
              lcd.print("4.");
            }
            delay(50);

            leaderboardView = setMenuState(leaderboardView, 4, 2);
            break;

          case 4:
            lcd.clear();

            lcd.setCursor(0, 0);
            if (noExistentHighscores > 3) {
              lcd.print("4." + leaderboardNames[3]);

              lcd.setCursor(11, 0);
              lcd.print(leaderboardScores[3]);

            } else {
              lcd.print("4.");
            }

            lcd.setCursor(0, 1);
            if (noExistentHighscores > 4) {
              lcd.print("5." + leaderboardNames[4]);

              lcd.setCursor(11, 1);
              lcd.print(leaderboardScores[4]);

            } else {
              lcd.print("5.");
            }
            delay(50);

            leaderboardView = setMenuState(leaderboardView, 5, 3);
            break;

          case 5:
            lcd.clear();

            lcd.setCursor(0, 0);
            if (noExistentHighscores > 4) {
              lcd.print("5." + leaderboardNames[4]);

              lcd.setCursor(11, 0);
              lcd.print(leaderboardScores[4]);

            } else {
              lcd.print("5.");
            }

            lcd.setCursor(0, 1);
            lcd.print("<- Go back");
            delay(50);
            leaderboardView = setMenuState(leaderboardView, leaderboardView, 4);
            
            break;
        }
      }

      if (swState == LOW) {
        menuState = 0;
        swState = HIGH;
        lastSwState = HIGH;
      }
    } else if (menuState == 3) {
      switch (settingsMenuView) {
        case 0:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> Enter name");
          
          lcd.setCursor(1, 1);
          lcd.print("Difficulty");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          settingsMenuView = setMenuState(settingsMenuView, 1, settingsMenuView);

          if (swState == LOW){
            yJoystickPosition = 0;
            settingsMenuView = 8;
            swState = HIGH;
            lastSwState = HIGH;
          }
          break;
        
        case 1:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Enter name");

          lcd.setCursor(0, 1);
          lcd.print("> Difficulty");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            settingsMenuView = 9; 
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, 2, 0);
          break;
        
        case 2:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Difficulty");
          
          lcd.setCursor(0, 1);
          lcd.print("> Brightness");
         
          lcd.setCursor(15, 0);
          lcd.write((byte)6);

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW) {
            brightnessItem = 0;
            settingsMenuView = 11; 
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, 4, 3);
          break;
        
        case 3:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> Difficulty");

          lcd.setCursor(1, 1);
          lcd.print("Brightness");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            settingsMenuView = 9; 
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, 2, 0);
          break;

        case 4:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Brightness");

          lcd.setCursor(0, 1);
          lcd.print("> Sound");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            settingsMenuView = 10; 
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, 6, 5);
          break;
        
        case 5:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> Brightness");

          lcd.setCursor(1, 1);
          lcd.print("Sound");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          if (swState == LOW){
            brightnessItem = 0;
            settingsMenuView = 11; 
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, 4, 3);
          break;
        
        case 6:
          lcd.clear();
          lcd.setCursor(1, 0);
          lcd.print("Sound");

          lcd.setCursor(0, 1);
          lcd.print("> Go Back");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          delay(50);

          if (swState == LOW){
            menuState = 0;
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, settingsMenuView, 7);
          break;
        
        case 7:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("> Sound");

          lcd.setCursor(1, 1);
          lcd.print("Go Back");
          
          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          delay(50);

          if (swState == LOW) {
            settingsMenuView = 10; 
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          settingsMenuView = setMenuState(settingsMenuView, 6, 5);
          break;

        case 8: // Name - Add / Edit
          lcd.createChar(0, currentLetterArrow);
          lcd.createChar(1, saveArrow);

          hasNameSet = true;
          setName();

          // yJoystickPosition: 0-4 letters of the name, 5 save
          if (swState == LOW && yJoystickPosition == 5) {
            hasChosenSaveOption = true;
            settingsMenuView = 0;
            swState = HIGH;
            lastSwState = HIGH;
          }

          break;

        case 9: // Difficulty
          lcd.createChar(0, currentLetterArrow);
          
          setDifficulty();
          if (swState == LOW) {
            settingsMenuView = 1;
            swState = HIGH;
            lastSwState = HIGH;
          }

          break;
        
        case 10: // Sound On/Off
          lcd.createChar(0, speaker);
          lcd.createChar(1, soundOn);
          lcd.createChar(2, soundOff);
          lcd.createChar(3, saveArrow);

          setSound();
          if (swState == LOW) { 
            EEPROM.put(2, activateSound); // Save Sound preferences to EEPROM
            settingsMenuView = 7;
            swState = HIGH;
            lastSwState = HIGH;
          }
          
          break;
        
        case 11: // Brightness Matrix/LCD 
          lcd.createChar(0, brightnessBar);
          lcd.createChar(1, brightnessPlaceholder);

          setBrightness();
          if (swState == LOW) { 
            // Go one menu up and save settings for Display brightness
            EEPROM.put(0, lcdBrightness);
            EEPROM.put(1, matrixBrightness);
            settingsMenuView = 5;
            swState = HIGH;
            lastSwState = HIGH;
          }

          break;
      }
    } else if (menuState == 4) {

      switch (infoMenuView) {
        case 0:
          lcd.clear();
          displayText(aboutSection[1], 1, 1, false);
          displayText(aboutSection[0], 1, 0, true);
          delay(50);

          infoMenuView = setMenuState(infoMenuView, 1, infoMenuView);

          break;
        
        case 1:
          lcd.clear();
          displayText(aboutSection[0], 1, 0, false);
          displayText(aboutSection[1], 1, 1, true);
          delay(50);

          infoMenuView = setMenuState(infoMenuView, 2, 0);

          break;
        
        case 2:
          lcd.clear();
          displayText(aboutSection[1], 1, 0, false);
          displayText(aboutSection[2], 1, 1, true);
          delay(50);

          infoMenuView = setMenuState(infoMenuView, 4, 3);
          
          break;
        
        case 3:
          lcd.clear();
          displayText(aboutSection[2], 1, 1, false);
          displayText(aboutSection[1], 1, 0, true);
          delay(50);

          infoMenuView = setMenuState(infoMenuView, 2, 0);

          break;
        
        case 4:
          lcd.clear();
          displayText(aboutSection[2], 1, 0, false);
          displayText(aboutSection[3], 1, 1, true);
          delay(50);

          infoMenuView = setMenuState(infoMenuView, 6, 5);

          break;

        case 5:
          lcd.clear();
          displayText(aboutSection[3], 1, 1, false);
          displayText(aboutSection[2], 1, 0, true);
          delay(50);

          infoMenuView = setMenuState(infoMenuView, 4, 3);
          
          break;

        case 6:
          infoMenuView = setMenuState(infoMenuView, infoMenuView, 7);

          lcd.clear();
          displayText(aboutSection[3], 1, 0, false);
          displayText(aboutSection[4], 1, 1, true);          
          delay(50);

          if (swState == LOW){
            menuState = 0;
            swState = HIGH;
            lastSwState = HIGH;
          }

          break;
        
        case 7:
          lcd.clear();
          displayText(aboutSection[4], 1, 1, false);
          displayText(aboutSection[3], 1, 0, true);
          delay(50);

          infoMenuView = setMenuState(infoMenuView, 6, 5);

          break;
      }
      
    } else if (menuState == 5) { // HOW TO PLAY 
      switch (htpMenuView) {
        case 0:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Classic Snake");

          lcd.setCursor(0, 1);
          lcd.print("game with minor");
          
          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          htpMenuView = setMenuState(htpMenuView, 1, htpMenuView); 

          break;
        
        case 1:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("changes:");
          
          lcd.setCursor(15, 0);
          lcd.write((byte)6);
          delay(50);

          lcd.setCursor(0, 1);
          lcd.print("The snake is");
          delay(50);

          lcd.setCursor(15, 1);
          lcd.write((byte)7);

          htpMenuView = setMenuState(htpMenuView, 2, 0); 

          break;
        
        case 2:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("one size and");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);

          lcd.setCursor(0, 1);
          lcd.print("max score is 10");

          lcd.setCursor(15, 1);
          lcd.write((byte)7);
          delay(50);

          htpMenuView = setMenuState(htpMenuView, 3, 1); 

          break;
        
        case 3:
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("   Have fun!");

          lcd.setCursor(15, 0);
          lcd.write((byte)6);

          lcd.setCursor(0, 1);
          lcd.print("Click to exit");
          delay(50);

          htpMenuView = setMenuState(htpMenuView, htpMenuView, 2); 

          break;
      }   

      if (swState == LOW){
        menuState = 0;
        swState = HIGH;
        lastSwState = HIGH;
      }

    }
  }
}

int setMenuState(int stay, int goDown, int goUp) {

  if (xValue < minThreshold && !xJoystickState) {
    indexText = 1;  // For a list with scrollable text
    xJoystickState = true;
    return goDown;
  }

  if (xValue > maxThreshold && !xJoystickState) {
    indexText = 1;
    xJoystickState = true;
    return goUp;
  }

  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
    return stay;
  }
}

void setName() {
  lcd.clear();

  lcd.setCursor(1, 0);
  lcd.print(playerName); 

  lcd.setCursor(12, 0);
  lcd.print("save"); 

  if (yJoystickPosition < 5) {
    // indication for the current letter
    lcd.setCursor(1 + yJoystickPosition, 1);
    lcd.write((byte)0);  
  } else {
    // indication for the save button
    lcd.setCursor(11, 0);
    lcd.write((byte)1);  
  }
  delay(50);

  movingBetweenPositions();
  if (yJoystickPosition < 5) {
    movingBetweenLetters();
  }
}

void movingBetweenLetters() {
  if (xValue < minThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {
    if (playerName[yJoystickPosition] != 'A') {
      playerName[yJoystickPosition] = (char)(playerName[yJoystickPosition] - 1);
    } else {
      playerName[yJoystickPosition] = 'Z';
    }

    xJoystickState = true;
  }
  
  if (xValue > maxThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {
    if (playerName[yJoystickPosition] != 'Z') {
      playerName[yJoystickPosition] = (char)(playerName[yJoystickPosition] + 1);
    } else {
      playerName[yJoystickPosition] = 'A';
    }

    xJoystickState = true;
  }

  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
  }
}

void movingBetweenPositions() {

  if (yValue < minThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    yJoystickPosition = yJoystickPosition > 0 ? yJoystickPosition - 1 : 0;
  }

  if (yValue > maxThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    yJoystickPosition = yJoystickPosition < 5 ? yJoystickPosition + 1  : 5;
  }

  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  }
}

void setDifficulty() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("EASY MEDIUM HELL");

  if (difficulty == 0) {
    lcd.setCursor(1, 1);
  } else if (difficulty == 1) {
    lcd.setCursor(9, 1);
  } else if (difficulty == 2) {
    lcd.setCursor(14, 1);
  }
  
  lcd.write((byte)0);
  delay(50);

  movingSelectedDifficulty();
}

void movingSelectedDifficulty() {

  if (yValue < minThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    difficulty = difficulty - 1;

    if (difficulty == -1) {
      difficulty = 2;
    }
  }

  if (yValue > maxThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    difficulty = (difficulty + 1) % 3;
  }

  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  }
}

void setSound() {
  // writing the custom speaker characters to the LCD 

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.write((byte)0);
  lcd.write((byte)1); // Volume active

  lcd.setCursor(4, 1);
  lcd.write((byte)0);
  lcd.write((byte)2); // Volume disabled

  if (activateSound) {
    lcd.setCursor(2, 0);
    lcd.write((byte)3); 
  } else {
    lcd.setCursor(2, 1);
    lcd.write((byte)3);
  }
  delay(50);

  // Move the arrow above the wanted option
  if (xValue < minThreshold && !xJoystickState) {
    xJoystickState = true;
    activateSound = false;
  }

  if (xValue > maxThreshold && !xJoystickState) {
    xJoystickState = true;
    activateSound = true;
  }

  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
  }
}

void setBrightness() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Brightness for:");

  lcd.setCursor(8, 1);
  lcd.print("<");

  lcd.setCursor(14, 1);
  lcd.print(">");

  lcd.setCursor(1, 1);
  if (brightnessItem == 0){
    lcd.print("LCD"); 
    analogWrite(lcdBrightnessControlPin, lcdBrightness);   
  } else {
    lcd.print("Matrix");
    lc.setIntensity(0, matrixBrightness);
  }

  setBrightnessDisplay();
  delay(50);
  
  changeDisplayedBrightnessLevel();
  
  // Movement between the two available sliders
  if (xValue < minThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {
    xJoystickState = true;

    if (brightnessItem == 0) {
      brightnessItem = 1;
    }
  }

  if (xValue > maxThreshold && yValue > minThreshold && yValue < maxThreshold && !xJoystickState) {
    xJoystickState = true;
    
    if (brightnessItem == 1) {
      brightnessItem = 0;
    }  
  }

  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoystickState = false;
  }
}

void setBrightnessDisplay() {
  byte displayedBrightnessLevel;
  
  if (brightnessItem == 0) {
    displayedBrightnessLevel = lcdBrightness / 51;
  } else {
    displayedBrightnessLevel = matrixBrightness / 3;
  }

  for (int i = 0 ; i < displayedBrightnessLevel; ++i) {
    lcd.setCursor(9 + i, 1);
    lcd.write((byte)0);
  }

  for (int i = displayedBrightnessLevel; i < 5; ++i) {
    lcd.setCursor(9 + i, 1);
    lcd.write((byte)1);
  }
}

// LCD range: 0-255 (6 increments of 51)
// Matrix range: 0-15 (6 increments of 3)
void changeDisplayedBrightnessLevel() {
  if (yValue < minThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;

    if (brightnessItem == 0) {
      if (lcdBrightness != 0) {
        lcdBrightness -= 51;
      }
    } else {
      if (matrixBrightness != 0) {
        matrixBrightness -= 3;
      }
    }
  }

  if (yValue > maxThreshold && xValue > minThreshold && xValue < maxThreshold && !yJoystickState) {
    yJoystickState = true;
    if (brightnessItem == 0) {
      if (lcdBrightness != 255) {
        lcdBrightness += 51;
      }
    } else {
      if (matrixBrightness != 15) {
        matrixBrightness += 3;
      }
    }
  }

  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  }
}

void displayText(String text, byte column, byte row, bool scrollable) {
  
  if (!scrollable) {

    lcd.setCursor(column, row);
    lcd.print(text.substring(0, 15));
  } else { 

    lcd.setCursor(column - 1, row);
    lcd.print(">");

    lcd.setCursor(column, row);
    
    if (text.length() <= 15) {
      lcd.print(text); 
    } else {
      String newText = text + "  ";

      if (indexText <= newText.length()) {
        scrollText(indexText, newText);
        indexText++;
      } else {
        indexText = 1;
      }
    }
  } 
}

// standard scroll text function
void scrollText(int startLetter, String message) {

  if (message.length() - startLetter > 15) {
    lcd.print(message.substring(startLetter, startLetter + 15));
  }
  else {    
    lcd.print(message.substring(startLetter, message.length()) + message.substring(0, 15 - message.length() + startLetter));
  } 

  delay(250);
}

// Initialization function for EEPROM
// Hard reset and rebase info
// It has to be called manually
void initializeEEPROM() {
  lcdBrightness = 102;
  matrixBrightness = 15;
  activateSound = true;
  noExistentHighscores = 0;

  EEPROM.put(0, lcdBrightness);
  EEPROM.put(1, matrixBrightness);
  EEPROM.put(2, activateSound);
  EEPROM.put(19, noExistentHighscores);
}

void setDataFromEEPROM() {
  lcdBrightness = EEPROM.read(0);
  matrixBrightness = EEPROM.read(1);
  activateSound = EEPROM.read(2);
}

void getLeaderboardFromEEPROM() {
  noExistentHighscores = EEPROM.read(19);
  delay(50);
  
  for (int i = 0; i < noExistentHighscores; ++i) {
    String currentPlayerName = "";
    long currentScore = 0;
  
    for (int j = 0; j < 5; ++j) {
      currentPlayerName += (char)EEPROM.read((i + 1) * 10 + j);
      delay(50);
    }

    byte firstByte = EEPROM.read((i + 1) * 10 + 6);
    delay(50);
    
    byte secondByte = EEPROM.read((i + 1) * 10 + 7);
    delay(50);

    currentScore = (firstByte * 256) + secondByte; // split in two for larger numbers, score supported until 65536

    leaderboardNames[i] = currentPlayerName;
    leaderboardScores[i] = currentScore;
  }
}

void applesBehaviour() {
  if (millis() - lastBlink > blinkTimeOn) {      
    lc.setLed(0, applePositionX, applePositionY, true);
  } 

  if (millis() - lastBlink > blinkTimeOff) {      
    lc.setLed(0, applePositionX, applePositionY, false);
    lastBlink = millis();
  }

  if (snakePositionY == applePositionY && snakePositionX == applePositionX) {
    applePositionY = random(matrixSize);
    applePositionX = random(matrixSize);
    score += 1;
  }
}

void snakeBehaviour() {
  
  // Move Up
  if (xValue > maxThreshold && !xJoystickState) {
    xJoystickState = true;
    snakePositionX = snakePositionX - 1;

    if (snakePositionX == 255) {
      snakePositionX = matrixSize - 1;
    }
  }

  // Move Down
  if (xValue < minThreshold && !xJoystickState) {
    xJoystickState = true;
    snakePositionX = snakePositionX + 1;

    if (snakePositionX == matrixSize) {
      snakePositionX = 0;
    }
  }

  // Move left
  if (yValue < minThreshold && !yJoystickState) {
    yJoystickState = true;
    snakePositionY = snakePositionY - 1;
    
    if (snakePositionY == 255) {
      snakePositionY = matrixSize - 1;
    }
  }

  // Move right
  if (yValue > maxThreshold && !yJoystickState) {
    yJoystickState = true;  
    snakePositionY = snakePositionY + 1;

    if (snakePositionY == matrixSize) {
      snakePositionY = 0;
    }
  }

  // Move diagonal left-up
  if (xValue > maxThreshold && yValue < minThreshold && !xJoystickState && !yJoystickState) {
    xJoystickState = true;
    yJoystickState = true;
    snakePositionX = snakePositionX - 1;

    if (snakePositionX == 255) {
      snakePositionX = matrixSize - 1;
    }

    snakePositionY = snakePositionY - 1;
    if (snakePositionY == 255) {
      snakePositionY = matrixSize - 1;
    }
  }

  // Move diagonal right-up
  if (xValue > maxThreshold && yValue > maxThreshold && !xJoystickState && !yJoystickState) { 
    xJoystickState = true;
    yJoystickState = true;
    snakePositionX = snakePositionX - 1;

    if (snakePositionX == 255) {
      snakePositionX = matrixSize - 1;
    }

    snakePositionY = snakePositionY + 1;
    if (snakePositionY == matrixSize) {
      snakePositionY = 0;
    }
  }

  // Move diagonal left-down
  if (xValue < minThreshold && !xJoystickState && yValue < minThreshold && !yJoystickState) {
    xJoystickState = true;
    yJoystickState = true;
    snakePositionX = snakePositionX + 1;

    if (snakePositionX == matrixSize) {
      snakePositionX = 0;
    }
    
    snakePositionY = snakePositionY - 1;
    if (snakePositionY == 255) {
      snakePositionY = matrixSize - 1;
    }
  }
  
  // Move diagonal right-down
  if (xValue < minThreshold && !xJoystickState && yValue > maxThreshold && !yJoystickState) {
    xJoystickState = true;
    yJoystickState = true;
    snakePositionX = snakePositionX + 1;

    if (snakePositionX == matrixSize) {
      snakePositionX = 0;
    }

    snakePositionY = snakePositionY + 1;
    if (snakePositionY == matrixSize) {
      snakePositionY = 0;
    }
  }

  if (xValue < maxThreshold && xValue > minThreshold && yValue < maxThreshold && yValue > minThreshold) {
    xJoystickState = false;
    yJoystickState = false;
  }
}

void displayInGameStats() {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (hasNameSet) {
    // Name was set in the settings menu
    lcd.print(playerName);
  } else {
    // If there was no name introduced will show ANONI on the in-game menu displayed on the LCD
    lcd.print("ANONI");
  }
  
  lcd.setCursor(6, 0);

  if (difficulty == 0) {
    lcd.print("Lvl: NOOB");
  } else if (difficulty == 1) {
    lcd.print("Lvl:MEDIUM");
  } else if (difficulty == 2) {
    lcd.print("Lvl: HELL");
  }

  lcd.setCursor(0, 1);

  for ( int i = 0; i < livesLeft; ++i) {
    lcd.setCursor(i, 1);
    lcd.write((byte)0);
  }
  
  lcd.setCursor(6, 1);
  lcd.print("EXP:" + String(score));
}

void showGameEndedSequence() {
  lcd.clear();
  
  // Game ended, Displaying stats and asking for Name for Leaderboard
  if (millis() - startGameOverSequence < gameOverDuration) {
    lcd.setCursor(0, 0);
    lcd.print("   GAME OVER!");

    lcd.setCursor(0, 1);
    lcd.print("Your Score:" + String(score));
  } else {
    if (!scoreMarked) {
      
      insertPosition = noExistentHighscores;

      while (insertPosition > 0 && score > leaderboardScores[insertPosition - 1]){
        --insertPosition;
      }
      
      if (insertPosition < 5) {
        if(!hasNameSet) {
            setName();

            if (swState == LOW && yJoystickPosition == 5) {
              hasNameSet = true;
              swState = HIGH;
              lastSwState = HIGH;
            }
        } else {
          if (!hasChosenSaveOption) {
            saveNameOption();

            if (swState == LOW){
              hasChosenSaveOption = true;
              swState = HIGH;
              lastSwState = HIGH;
            }
          } else { 
            if (insertPosition == noExistentHighscores) {
              noExistentHighscores += 1;
              leaderboardNames[insertPosition] = playerName;
              leaderboardScores[insertPosition] = score;

            } else {
              for (int i = noExistentHighscores - 1; i > insertPosition; --i) {
                leaderboardNames[i] = leaderboardNames[i - 1];
                leaderboardScores[i] = leaderboardScores[i - 1];
              } 

              leaderboardNames[insertPosition] = playerName;
              leaderboardScores[insertPosition] = score;
              noExistentHighscores ++;
            }
            saveLeaderboardToEEPROM();
            scoreMarked = true;

            if (!saveName) {
              playerName = "AAAAA";
              hasNameSet = false;
            }
          }
        }
      } else {
        scoreMarked = true;        
      }
    }
  }
}

void saveLeaderboardToEEPROM() {
  EEPROM.put(19, noExistentHighscores);

  for (int i = 1; i <= noExistentHighscores; ++i) {
    for (int j = 0; j < 5; ++j) {
      EEPROM.put(i * 10 + j, leaderboardNames[i - 1][j]);
    }

    int firstByte = highByte (leaderboardScores[i - 1]);
    int secondByte = lowByte (leaderboardScores[i - 1]); 

    EEPROM.put(i * 10 + 6, firstByte);
    EEPROM.put(i * 10 + 7, secondByte);
  }
}

void resetMatrix() {
  for (int i = 0; i < matrixSize; ++i) {
    for (int j = 0; j < matrixSize; ++j) {
      lc.setLed(0, i, j, true);
      delay(20);
    }
  }

  for (int i = 0; i < matrixSize; ++i) {
    for (int j = 0; j < matrixSize; ++j) {
      lc.setLed(0, i, j, false);
      delay(20);
    }
  }
}

// Manually tuning a sound for Game Over
// Not original 
void gameOverSound() {
  tone(buzzer, 500, 200);
  delay(100);
  tone(buzzer, 1200, 200);
  delay(100);
  tone(buzzer, 300, 200);
  delay(100);
  tone(buzzer, 1000, 200);
  delay(100);
  tone(buzzer, 400, 200);
  delay(100);
  tone(buzzer, 100, 200);
  delay(100);
}

void saveNameOption() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Save new name?");

  if (saveName) {
    lcd.setCursor(0, 1);
    lcd.print("   >Yes    No");
  } else {
    lcd.setCursor(0, 1);
    lcd.print("    Yes   >No");
  }
   
  // Move between options
  if (yValue < minThreshold && !yJoystickState) {
    yJoystickState = true;
    saveName = true;
  }

  if (yValue > maxThreshold && !yJoystickState) {
    yJoystickState = true;
    saveName = false;
  }

  if (yValue < maxThreshold && yValue > minThreshold) {
    yJoystickState = false;
  } 
}

// Display the custom images on the Matrix display
void printImageOnMatrix(const byte* image) {
  
  for (int i = 0; i < matrixSize; ++i) {
    for (int j = 0; j < matrixSize; ++j) {
      lc.setLed(0, i, j, false);
    }
  }
  
  for (int i = 0; i < matrixSize; ++i) {
    for (int j = 0; j < matrixSize; ++j) {
      lc.setLed(0, i, j, bitRead(image[i], 7 - j));
    }
  }
}
