#include "Arduino.h"
#include "SDR_SUGV.h"
#include "Protocol.h"
#include "Localization.h"
#include "Sensing.h"
#include "Action.h"
#include <LiquidCrystal.h>

int stopFlag = 0;
int autoFlag = -1;   // Auto or Manual Control

unsigned long sonarClock = 0;
unsigned long lineClock = 0;
unsigned long odometerClock = 0;
unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const int rs=29, en=31, d4=33, d5=35, d6=37, d7=39;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
  initialization();
  lcd.begin(5, 1);
  
}

int value;

void loop() {
  currentMillis = millis();
  value = analogRead(A0);
  Serial.println(value);
  if (value > 200) {
    digitalWrite(50, LOW);
    digitalWrite(52, LOW);
  } else {
    digitalWrite(50, HIGH);
    digitalWrite(52, HIGH);
  }
  
  if (sonarAverage < SONAR_STOP_DISTANCE) {
    lcd.print("MOVE!           ");
  }  else {
    lcd.print("                "); 
  }
  
  updateCommand();
  updateSensors();  
  updatePlan();
  updateAction();
  previousMillis = currentMillis;  
}

void updatePlan(){
  if (autoFlag == 1){ //Auto-driving    
  }else if (autoFlag == 0){
  }else{
  }
  if (stopFlag){
    speedR = 0; speedL = 0;
  }
}

void updateAction(){

  updateMotor();
}



void updateSensors(){
  updateLineSensor(); 
  sonar = updateSonar();
  averageSonar();
  updateOdometer();
  updateSpeedmeter();
}

int sonarAverage;
int sonarTotal;
int sonarCounts;

void averageSonar() {
  if (sonarCounts > 50) {
    sonarCounts = 1;
    sonarTotal = 0;
  }
  sonarTotal = sonarTotal + sonar;
  sonarAverage = sonarTotal / sonarCounts;
  sonarCounts++;
}




void initialization(){
  currentMillis = millis();
  init_communication();
  init_pin();
  sonarCounts = 1;
  sonarTotal = 0;
  
}

void init_communication(){
  Serial.begin(115200); Serial.println("Program Start...v04");
  init_buffer(); 
}

void init_pin(){
  //Motor pin setting
  pinMode(MOTOR_LEFT_ENABLE, OUTPUT); pinMode(MOTOR_LEFT_A, OUTPUT); pinMode(MOTOR_LEFT_B, OUTPUT);
  pinMode(MOTOR_RIGHT_ENABLE, OUTPUT); pinMode(MOTOR_RIGHT_A, OUTPUT); pinMode(MOTOR_RIGHT_B, OUTPUT);
  //Sonar pin setting
  pinMode(SONAR_ECHO, INPUT); pinMode(SONAR_TRIG, OUTPUT);
  pinMode(LINE_LEFT, INPUT_PULLUP); pinMode(LINE_RIGHT, INPUT_PULLUP); 
  attachInterrupt(0, rightOdometer, CHANGE);
  attachInterrupt(1, leftOdometer, CHANGE);
  // setting up the headlight LED pins
  pinMode(50, OUTPUT); pinMode(52, OUTPUT);
  pinMode(A0, INPUT);
}



