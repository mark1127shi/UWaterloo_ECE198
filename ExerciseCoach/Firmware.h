#pragma once // only program once
#include <Arduino.h>
#include <Wire.h>
// Wire provides I2c communication which sends data through SDA and SCL
#include <hd44780.h>
#include <hd44780ioClass/hd44780_I2Cexp.h>
#include <EEPROM.h> // Electrically Erasable Programmable Read-Only Memory which saved data in the Arduino

// declaration
hd44780_I2Cexp lcd;
const int joystickPin = A2; // and this is x which is left and right
const int joystickButtonPin = 6;
bool joystickReady = false;
int menuIndex = 0;
char name[8] = "User";
int lastButtonReading = HIGH; //previous
int stableButtonState = HIGH;// we've accepted
unsigned long buttonChangedAt = 0; // raw reading
// time of the last raw input change which help to adjudt time
bool onModePage = false; //show which
int selectedMode = 1; // which mode which will be equal to menuIndex which is a function I writed below
//std which is normal one
int trainingSets = 2;
int repsPerSet = 3;
int restSeconds = 10;
bool onTrainingPage = false; //same to onModePage
int currentSet = 1; //defalt
int currentRep = 0;
const int accelerometerXPin = A0;
const int accelerometerYPin = A1;
const int accelerometerZPin = A3;
unsigned long lastSensorPrint = 0;
int x = 0;
int y = 0;
int z = 0;
unsigned int reps = 0;  // reps
int stage = 0;
bool isResting = false;
unsigned long restStartedAt = 0;
struct WorkoutRecord { // struct gives different type of data stored in here.
    uint32_t marker;
    int sets;
    int repsEach;
};

const uint32_t RECORD_MARKER = 0x198C0001; // random number
const int RECORD_ADDRESS = 0;

WorkoutRecord lastWorkout = {};
bool hasLastWorkout = false;
bool onRecordPage = false;

void saveWorkout();
void loadWorkout();
void showLastWorkout();
void displayReps();
void resetWorkout();
void printTilt();
void readAccelerometer();
void showTraining();


void getUserName() {
  Serial.println("Enter your name, up to 7 characters.");
  Serial.println("Use New Line to enter");
  Serial.println("Or press the joystick to use User.");

  while (Serial.available() == 0) {
    if (digitalRead(joystickButtonPin) == LOW) {
      return;
    }
    delay(10);
  }

  String input = Serial.readStringUntil('\n');

  if (input.length() > 0) {
    input.toCharArray(name, sizeof(name)); // copies up to seven characters into name
  }// Notice that sizeof() is to show whole capacity
}
  


void showMenu(const char *userName) {
  lcd.setCursor(0, 0);
  lcd.print("                "); //16
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("Welcome, ");
  lcd.print(userName);  
  lcd.setCursor(0, 1);

  if (menuIndex == 0) {
    lcd.print(">Start training");
  } else if (menuIndex == 1) {
    lcd.print(">Select mode");
  } else {
    lcd.print(">Last record");
  }
}

void showModeMenu(){
  lcd.setCursor(0,0);
  lcd.print("Choose mode     ");
  lcd.setCursor(0,1);
  lcd.print("                ");

  lcd.setCursor(0,1);
  if(menuIndex ==0){
    lcd.print(">Short 1x3");
  }else if(menuIndex ==1){
    lcd.print(">Standard 2x3");
  }else{
    lcd.print(">Extended 3x5");
  }
}

void showTraining() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");

  lcd.setCursor(0, 0);
  lcd.print("Set ");
  lcd.print(currentSet);
  lcd.print("/");
  lcd.print(trainingSets);

  lcd.print("Rep ");
  lcd.print(reps);
  lcd.print("/");
  lcd.print(repsPerSet);

  lcd.setCursor(0, 1);
  lcd.print("Click: back");
}

void refreshScreen(){
  if(onTrainingPage){
    displayReps();
  }else if(onModePage){
    showModeMenu();
  }else{
    showMenu(name);
  }
}

void resetWorkout(){
  reps = 0;
  stage = 0;
  currentSet = 1;
  isResting = false;
  displayReps();
}

bool joystickPressed() {
  int reading = digitalRead(joystickButtonPin);
  unsigned long now = millis();// return number of milliseconds since board began running the program

  if (reading != lastButtonReading){ //->change button
    buttonChangedAt = now;
  }

  lastButtonReading = reading;
    if (now - buttonChangedAt >= 30) { // whether 30ms passed
      if (reading != stableButtonState) { // whether changed status
        stableButtonState = reading;

        if (stableButtonState == LOW) {
          return true;
        }
      }
  }

  return false;
}

void selectMenu() {
  if (onRecordPage) {
    onRecordPage = false;
    menuIndex = 2;
    refreshScreen();
    return;
  }

  if(onTrainingPage){
    onTrainingPage = false;
    menuIndex =0;
    refreshScreen();
    return;
  }

  if(onModePage){
    selectedMode = menuIndex;
    if(selectedMode ==0){
      trainingSets = 1;
      repsPerSet = 3;
      restSeconds = 10;
    }else if (selectedMode ==1){
      trainingSets = 2;
      repsPerSet = 3;
      restSeconds = 10;
    } else {
      trainingSets = 3;
      repsPerSet = 5;
      restSeconds = 15;
    }
    Serial.print("Sets: ");
    Serial.println(trainingSets);

    Serial.print("Reps per set: ");
    Serial.println(repsPerSet);

    Serial.print("Rest seconds: ");
    Serial.println(restSeconds);

    onModePage = false;
    menuIndex = 1;
    lcd.setCursor(0,0);
    lcd.print("Main Menu       ");
    lcd.setCursor(0,1);
    lcd.print("Loading...      ");
    delay(500);
    refreshScreen();
    return;

  }
  

  if (menuIndex == 0) {
    currentSet = 1;
    currentRep = 0;

    onModePage = false;
    onTrainingPage = true;
    lcd.setCursor(0, 0);
    lcd.print("Start Training  ");
    lcd.setCursor(0, 1);
    lcd.print("Strong! (^_^)   ");
    delay(2000);
    refreshScreen();
    resetWorkout();
  }else if (menuIndex == 1) {
      Serial.println("Selected: Select mode");
      onModePage = true;
      menuIndex = selectedMode;
      lcd.setCursor(0,0);
      lcd.print("Mode Page       ");
      lcd.setCursor(0,1);
      lcd.print("Loading...      ");
      delay(500);
      refreshScreen();
  }else {
    onRecordPage = true;
    showLastWorkout();
  }
}

void saveWorkout() {
  lastWorkout.marker = RECORD_MARKER;
  lastWorkout.sets = trainingSets;
  lastWorkout.repsEach = repsPerSet;

  EEPROM.put(RECORD_ADDRESS, lastWorkout); // This is read data from EEPROM
  hasLastWorkout = true;
}

void loadWorkout() {
  EEPROM.get(RECORD_ADDRESS, lastWorkout);

  // Accept only records matching one of three modes.
  bool validPlan =
    (lastWorkout.sets == 1 && lastWorkout.repsEach == 3) ||
    (lastWorkout.sets == 2 && lastWorkout.repsEach == 3) ||
    (lastWorkout.sets == 3 && lastWorkout.repsEach == 5);

  hasLastWorkout =
    lastWorkout.marker == RECORD_MARKER && validPlan;
}

void showLastWorkout() {
  lcd.clear();

  if (!hasLastWorkout) {
    lcd.print("No saved workout");
    lcd.setCursor(0, 1);
    lcd.print("Click: back");
    return;
  }

  lcd.print("Last: ");
  lcd.print(lastWorkout.sets);
  lcd.print("x");
  lcd.print(lastWorkout.repsEach);

  lcd.setCursor(0, 1);
  lcd.print("Total reps: ");
  lcd.print(lastWorkout.sets * lastWorkout.repsEach);
}

void coachSetup(){
  Serial.begin(115200);
  analogReadResolution(14); //set ADC to 14-bit resolution
  loadWorkout();
    
  pinMode(joystickButtonPin, INPUT_PULLUP);// configure Pin -> input

  while (!Serial) {
    // Wait for the computer to open the serial connection.
  }
  Serial.println("Exercise Coach started");
  int lcdStatus = lcd.begin(16,2);
  //button more like a resister and
  //pressed-->low, released ->high

  //Accident
  if( lcdStatus != 0){
    Serial.println("LCD initialization failed");// this is check whether LCD works or not
    return;
  }

  lcd.setCursor(0,0);
  lcd.print("Enter name on PC");
  lcd.setCursor(0,1);
  lcd.print("Click: use User");
  
    
  getUserName();
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("Exercise Coach");

  lcd.setCursor(0, 1);
  lcd.print("Welcome, ");
  lcd.print(name);
  delay(2000); //2s
  showMenu(name);

  //initialize those data in button
  lastButtonReading = digitalRead(joystickButtonPin);
  stableButtonState = lastButtonReading;
  buttonChangedAt = millis();
  /*
  lcd.setCursor(0, 0); // set the stater and (x,y)
  lcd.print("Exercise Coach");// context

  lcd.setCursor(0, 1);
  lcd.print("Ready");*/
  // This is LCD test


  if (isResting) {
    unsigned long elapsed = millis() - restStartedAt;
    unsigned long restDuration = restSeconds * 1000UL;

    if (elapsed >= restDuration) {
      isResting = false;
      ++currentSet;
      reps = 0;
      stage = 0;
      displayReps();
      } else {
        unsigned long secondsLeft =
          (restDuration - elapsed + 999UL) / 1000UL;

        lcd.setCursor(0, 0);
        lcd.print("Rest            ");
        lcd.setCursor(0, 1);
        lcd.print("You made this   ");        
        lcd.setCursor(0, 1);
        lcd.print(secondsLeft);
        lcd.print(" seconds left");
      }

      delay(20);
      return;
    }

    readAccelerometer();
    printTilt();
    return;
}

void readAccelerometer(){
  unsigned long now = millis();
  if(now - lastSensorPrint < 200){
    return;
  }
  lastSensorPrint = now; 
  // Read
  float xReading = analogRead(A0);
  float yReading = analogRead(A1);
  float zReading = analogRead(A3);
  x = analogRead(A0);
  y = analogRead(A1);
  z = analogRead(A3);
}

void displayReps() {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("               ");
  lcd.setCursor(0,0);

  if (reps >= repsPerSet && currentSet >= trainingSets){
    lcd.print("Workout Done!   ");
  }else if(reps >= repsPerSet){
    lcd.print("Set complete    ");
  }

  showTraining();
  /*
  lcd.setCursor(0, 1);  // First column, second row.
  lcd.print("Reps: ");
  lcd.print(reps);
  lcd.print("/"); 
  lcd.print(repsPerSet);
  */
}


void countRep(float angle) {
  if (isResting || reps >= repsPerSet) {
    return;
  }

  if (stage == 0 && angle <= 20.0f) {
    stage = 1;
  }
  else if (stage == 1 && angle >= 60.0f) {
    stage = 2;
  }
  else if (stage == 2 && angle <= 20.0f) {
    ++reps;
    stage = 1;
    displayReps();

    Serial.print("Reps: ");
    Serial.println(reps);

    if (reps >= repsPerSet) {
      if (currentSet < trainingSets) {
        // More sets remain: start rest.
        isResting = true;
        restStartedAt = millis();
      } else {
      // Final set complete: show message, then menu.
        saveWorkout();
        lcd.setCursor(0, 0);
        lcd.print("Workout done!   ");
        lcd.setCursor(0, 1);
        lcd.print("Great job!      ");
        delay(2000);

        isResting = false;
        onTrainingPage = false;
        onModePage = false;
        menuIndex = 0;
        joystickReady = false;
        refreshScreen();
        }
      }
    }
}


void printTilt(){
  float ax = (x - 5575.0f) / 1175.0f;
  float ay = (y - 5540.0f) / 1160.0f;
  float az = (z - 5535.0f) / 1085.0f;
  float magnitude = sqrtf(ax * ax + ay * ay + az * az);

  if(magnitude <= 0.01f){
    return;
  }

  float cosine = -az/ magnitude;
  cosine = constrain(cosine, -1.0f,1.0f);

  float angle = acosf(cosine) * 180.0f /PI;

  countRep(angle);

}



void coachLoop(){
  if (joystickPressed()) {
    selectMenu();
    joystickReady = false; //must return central area [6000,10000]
    return;
  }
  if (onTrainingPage) {
    if (isResting) {
      unsigned long elapsed = millis() - restStartedAt;
      unsigned long restDuration = restSeconds * 1000UL;

      if (elapsed >= restDuration) {
          isResting = false;
          ++currentSet;
          reps = 0;
          stage = 0;
          displayReps();
      } else {
          unsigned long secondsLeft =
          (restDuration - elapsed + 999UL) / 1000UL;

          lcd.setCursor(0, 0);
          lcd.print("Rest            ");
          lcd.setCursor(0, 1);
          lcd.print("                ");
          lcd.setCursor(0, 1);
          lcd.print(secondsLeft);
          lcd.print(" seconds left");
        }

        delay(20);
        return; // Skip rep counting during rest.
    }

    readAccelerometer();
    printTilt();
    return;
}

  int joystickValue = analogRead(joystickPin);

  if (joystickValue >= 6000 && joystickValue <= 10000) {
      joystickReady = true;
    }
  if (joystickReady){
    if (joystickValue < 4000) {
      Serial.println("LEFT");

      menuIndex--;
      if (menuIndex < 0){
        menuIndex = 2;
      }

      joystickReady = false;
      refreshScreen();

    }else if (joystickValue > 12000) {
      Serial.println("RIGHT");

        menuIndex++;
        if (menuIndex > 2) {
          menuIndex = 0;
        }

        joystickReady = false;
        refreshScreen();

    }
  }
  delay(20);
  
  /*
  Serial.println(joystickValue); //A2
  if (joystickValue < 4000){
    Serial.println("Left");
  }else if( joystickValue > 12000){
    Serial.println("Right");
  }else{
    Serial.println("Center");
  }

  delay(200); // 0.2s
  //This is test for joystick if it works or not.
  */
}
