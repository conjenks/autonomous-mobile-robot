#include "Arduino.h"
#include "Action.h"
#include "Sensing.h"
#include "Protocol.h"
#include "Localization.h"
#include "SDR_SUGV.h"

void stop();

unsigned long lastMessageMillis = 0;

void updateMotor() {

  if (autoFlag == -1) { // manual driving
    if (currentMillis - lastMessageMillis > 2500) { // print status every 2.5 seconds in manual driving
      statusCommand();
      lastMessageMillis = currentMillis;
    }

    if (sonarAverage > SONAR_STOP_DISTANCE || (mSpeedR == -255 && mSpeedL == -255)) {
      if ( mSpeedR > 0) {
        digitalWrite(MOTOR_RIGHT_A, HIGH); 
        digitalWrite(MOTOR_RIGHT_B, LOW); 
        analogWrite(MOTOR_RIGHT_ENABLE, mSpeedR);
      } 
      else if ( mSpeedR < 0) { // REVERSE?
        digitalWrite(MOTOR_RIGHT_A, LOW); 
        digitalWrite(MOTOR_RIGHT_B, HIGH); 
        analogWrite(MOTOR_RIGHT_ENABLE, abs(mSpeedR));
      } 
      else {
        digitalWrite(MOTOR_RIGHT_A, LOW); 
        digitalWrite(MOTOR_RIGHT_B, LOW); 
        analogWrite(MOTOR_RIGHT_ENABLE, 0);
      }
      if ( mSpeedL > 0) {
        digitalWrite(MOTOR_LEFT_A, HIGH); 
        digitalWrite(MOTOR_LEFT_B, LOW); 
        analogWrite(MOTOR_LEFT_ENABLE, mSpeedL);
      } 
      else if ( mSpeedL < 0) {
        digitalWrite(MOTOR_LEFT_A, LOW); 
        digitalWrite(MOTOR_LEFT_B, HIGH ); 
        analogWrite(MOTOR_LEFT_ENABLE, abs(mSpeedL));
      } 
      else {
        digitalWrite(MOTOR_LEFT_A, LOW); 
        digitalWrite(MOTOR_LEFT_B, LOW); 
        analogWrite(MOTOR_LEFT_ENABLE, 0);
      }
    } 
    else { // stop
      digitalWrite(MOTOR_RIGHT_A, LOW); 
      digitalWrite(MOTOR_RIGHT_B, LOW); 
      analogWrite(MOTOR_RIGHT_ENABLE, 0);
      digitalWrite(MOTOR_LEFT_A, LOW); 
      digitalWrite(MOTOR_LEFT_B, LOW); 
      analogWrite(MOTOR_LEFT_ENABLE, 0);
    }

  }
  if (autoFlag == 1) { // auto driving here

    if (sonarAverage < SONAR_STOP_DISTANCE) { // stop-- WILL NEED TO BE ADJUSTED - NOISE IS NOT FULLY ACCOUNTED FOR
      Serial.println("obstacle detected -- STOPPED");
      stop();
    }
    else if (lineR == 1) { // turn right while we're over the left line
      Serial.println("right line detected");
      stop();
      delay(500);
      digitalWrite(MOTOR_RIGHT_A, HIGH); 
      digitalWrite(MOTOR_RIGHT_B, LOW); 
      analogWrite(MOTOR_RIGHT_ENABLE, -255);
      digitalWrite(MOTOR_LEFT_A, HIGH); 
      digitalWrite(MOTOR_LEFT_B, LOW); 
      analogWrite(MOTOR_LEFT_ENABLE, 0);
      stop();
      delay(500);
      digitalWrite(MOTOR_RIGHT_A, HIGH); 
      digitalWrite(MOTOR_RIGHT_B, LOW); 
      analogWrite(MOTOR_RIGHT_ENABLE, -255);
      digitalWrite(MOTOR_LEFT_A, HIGH);
      digitalWrite(MOTOR_LEFT_B, LOW); 
      analogWrite(MOTOR_LEFT_ENABLE, 255);
      delay(750);
      stop();
    }
    else if (lineL == 1) { // turn left while we're over the right line
      Serial.println("left line detected");
      stop();
      delay(500);
      digitalWrite(MOTOR_RIGHT_A, HIGH); 
      digitalWrite(MOTOR_RIGHT_B, LOW); 
      analogWrite(MOTOR_RIGHT_ENABLE, 0);
      digitalWrite(MOTOR_LEFT_A, HIGH); 
      digitalWrite(MOTOR_LEFT_B, LOW); 
      analogWrite(MOTOR_LEFT_ENABLE, -255);
      stop();
      delay(500);
      digitalWrite(MOTOR_RIGHT_A, HIGH); 
      digitalWrite(MOTOR_RIGHT_B, LOW); 
      analogWrite(MOTOR_RIGHT_ENABLE, 255);
      digitalWrite(MOTOR_LEFT_A, HIGH); 
      digitalWrite(MOTOR_LEFT_B, LOW); 
      analogWrite(MOTOR_LEFT_ENABLE, -255);
      delay(750);
      stop();
    }
    else { // no constraints, move forward
      digitalWrite(MOTOR_RIGHT_A, HIGH); 
      digitalWrite(MOTOR_RIGHT_B, LOW); 
      analogWrite(MOTOR_RIGHT_ENABLE, 255);
      digitalWrite(MOTOR_LEFT_A, HIGH); 
      digitalWrite(MOTOR_LEFT_B, LOW); 
      analogWrite(MOTOR_LEFT_ENABLE, 255);
    }

  }
}

void stop() {
  digitalWrite(MOTOR_RIGHT_A, LOW); 
  digitalWrite(MOTOR_RIGHT_B, LOW); 
  analogWrite(MOTOR_RIGHT_ENABLE, 0);
  digitalWrite(MOTOR_LEFT_A, LOW); 
  digitalWrite(MOTOR_LEFT_B, LOW); 
  analogWrite(MOTOR_LEFT_ENABLE, 0);
}











