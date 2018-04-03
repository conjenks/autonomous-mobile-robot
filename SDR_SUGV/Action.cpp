#include "Arduino.h"
#include "Action.h"
#include "Sensing.h"
#include "Protocol.h"
#include "Localization.h"
#include "SDR_SUGV.h"


void updateMotor() {

  if (autoFlag == -1) { // manual driving

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
      digitalWrite(MOTOR_RIGHT_A, LOW); 
      digitalWrite(MOTOR_RIGHT_B, LOW); 
      analogWrite(MOTOR_RIGHT_ENABLE, 0);
      digitalWrite(MOTOR_LEFT_A, LOW); 
      digitalWrite(MOTOR_LEFT_B, LOW); 
      analogWrite(MOTOR_LEFT_ENABLE, 0);
    }
    else if (lineL == 1) { // we're over the right line, turn left for .3 seconds then move straight
    Serial.println("right line auto called");
      digitalWrite(MOTOR_RIGHT_A, HIGH); 
      digitalWrite(MOTOR_RIGHT_B, LOW); 
      analogWrite(MOTOR_RIGHT_ENABLE, 255);
      digitalWrite(MOTOR_LEFT_A, HIGH); 
      digitalWrite(MOTOR_LEFT_B, LOW); 
      analogWrite(MOTOR_LEFT_ENABLE, 100);
      delay(300);
      digitalWrite(MOTOR_RIGHT_A, HIGH); 
      digitalWrite(MOTOR_RIGHT_B, LOW); 
      analogWrite(MOTOR_RIGHT_ENABLE, 255);
      digitalWrite(MOTOR_LEFT_A, HIGH); 
      digitalWrite(MOTOR_LEFT_B, LOW); 
      analogWrite(MOTOR_LEFT_ENABLE, 255);
    }
    else if (lineR == 1) { // we're over the left line, turn right for .3 seconds then move straight
      Serial.println("left line auto called");
      digitalWrite(MOTOR_RIGHT_A, HIGH); 
      digitalWrite(MOTOR_RIGHT_B, LOW); 
      analogWrite(MOTOR_RIGHT_ENABLE, 100);
      digitalWrite(MOTOR_LEFT_A, HIGH); 
      digitalWrite(MOTOR_LEFT_B, LOW); 
      analogWrite(MOTOR_LEFT_ENABLE, 255);
      delay(300);
      digitalWrite(MOTOR_RIGHT_A, HIGH); 
      digitalWrite(MOTOR_RIGHT_B, LOW); 
      analogWrite(MOTOR_RIGHT_ENABLE, 255);
      digitalWrite(MOTOR_LEFT_A, HIGH); 
      digitalWrite(MOTOR_LEFT_B, LOW); 
      analogWrite(MOTOR_LEFT_ENABLE, 255);
    }
    else { // no constraints, move forward
      Serial.println("auto moving forward");
      digitalWrite(MOTOR_RIGHT_A, HIGH); 
      digitalWrite(MOTOR_RIGHT_B, LOW); 
      analogWrite(MOTOR_RIGHT_ENABLE, 255);
      digitalWrite(MOTOR_LEFT_A, HIGH); 
      digitalWrite(MOTOR_LEFT_B, LOW); 
      analogWrite(MOTOR_LEFT_ENABLE, 255);
    }

  }
}




