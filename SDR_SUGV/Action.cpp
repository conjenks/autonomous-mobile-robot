#include "Arduino.h"
#include "Action.h"
#include "Sensing.h"
#include "Protocol.h"
#include "Localization.h"
#include "SDR_SUGV.h"


void updateMotor(){
  if( mSpeedR > 0){
    digitalWrite(MOTOR_RIGHT_A, HIGH); digitalWrite(MOTOR_RIGHT_B, LOW); analogWrite(MOTOR_RIGHT_ENABLE, mSpeedR); 
  }else if ( mSpeedR < 0){ // REVERSE?
    digitalWrite(MOTOR_RIGHT_A, LOW); digitalWrite(MOTOR_RIGHT_B, HIGH); analogWrite(MOTOR_RIGHT_ENABLE, abs(mSpeedR)); 
  }else {
    digitalWrite(MOTOR_RIGHT_A, LOW); digitalWrite(MOTOR_RIGHT_B, LOW); analogWrite(MOTOR_RIGHT_ENABLE, 0);    
  }
  if( mSpeedL > 0){
    digitalWrite(MOTOR_LEFT_A, HIGH); digitalWrite(MOTOR_LEFT_B, LOW); analogWrite(MOTOR_LEFT_ENABLE, mSpeedL);     
  }else if ( mSpeedL < 0){
    digitalWrite(MOTOR_LEFT_A, LOW); digitalWrite(MOTOR_LEFT_B, HIGH ); analogWrite(MOTOR_LEFT_ENABLE, abs(mSpeedL)); 
  }else {
    digitalWrite(MOTOR_LEFT_A, LOW); digitalWrite(MOTOR_LEFT_B, LOW); analogWrite(MOTOR_LEFT_ENABLE, 0);         
  }
}


