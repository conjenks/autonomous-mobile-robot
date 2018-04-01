#include "Arduino.h"
#include "Protocol.h"
#include "Localization.h"
#include "Sensing.h"
#include "SDR_SUGV.h"
#include "Action.h"

char msgBuffer[MESSAGE_BUFFER_SIZE]; 
int msgBufferPointer = 0;

int mSpeedR = 0; // control message from the remote
int mSpeedL = 0; // control message from the remote


void setMovement(char);
void checkAutoManual(char);
void evaluateStringCommand();
void evaluateBinaryCommand();
void echoCommand();

void init_buffer(){
  for (int i = 0; i < MESSAGE_BUFFER_SIZE; i++){
    msgBuffer[i] = 0x00;
  }
  msgBufferPointer = 0;
}


void updateCommand(){
  if (Serial.available() > 0){
    char tmpChar = Serial.read(); // get first char from Serial
    setMovement(tmpChar);
    checkAutoManual(tmpChar);
    if ((msgBufferPointer == 0)&&(tmpChar == '<')){           // '<' 0x3C 
      msgBuffer[msgBufferPointer] = tmpChar; 
      msgBufferPointer++;
    }
    else if (msgBufferPointer == 1){
      if  ((tmpChar == '@')||(tmpChar == '!')){     // '!'  (0x21 ==> Binary Format)or '@' (0x40 ==> String format)
        msgBuffer[msgBufferPointer] = tmpChar; 
        msgBufferPointer++;      
      }
      else{
        msgBufferPointer = 0;
      }
    }
    else if (msgBufferPointer == 2){
      if (tmpChar == ' '){     // ' ' (0x20 ==> Space)
        msgBuffer[msgBufferPointer] = tmpChar; 
        msgBufferPointer++;      
      }
      else{
        msgBufferPointer = 0;
      }
    }
    else if (msgBufferPointer > MESSAGE_BUFFER_SIZE){ 
      msgBufferPointer = 0; 
      if ( tmpChar == '<') {
        msgBuffer[0] = tmpChar;    
        msgBufferPointer = 1;          
      }
    }
    else if (msgBufferPointer > 2) {
      if (( msgBuffer[1] == '@')&& (tmpChar == '>')) {
        //echoCommand();
        evaluateStringCommand();
        msgBufferPointer = 0;        
      }
      else if  (( msgBuffer[1] == '!')&& (msgBuffer[3] == msgBufferPointer )) {
        evaluateBinaryCommand();
        msgBufferPointer = 0;
      }
      else{
        msgBuffer[msgBufferPointer] = tmpChar;
        msgBufferPointer++;
      }
    }
  }  
}


void setMovement(char c) {
  if (c == 't') {
    Serial.println("forward");
    mSpeedR = 255;
    mSpeedL = 255;
  } 
  else if (c == 'h') { // TURN RIGHT
    updateMotor();
    Serial.println("left");
    mSpeedR = 255;
    mSpeedL = 100; 
  } 
  else if (c =='f') { // TURN LEFT
    updateMotor();
    Serial.println("right");
    mSpeedR = 100;
    mSpeedL = 255; 
  } 
  else if (c == 'g') {
    Serial.println("stop");
    mSpeedR = 0;
    mSpeedL = 0;
  }
  else if (c == 'v') {
    mSpeedR = 0;
    mSpeedL = 0;
    updateMotor();
    Serial.println("reverse");
    mSpeedR = -255;
    mSpeedL = -255;
  }
  statusCommand();
}

void checkAutoManual(char c) {
  if (c == 's') {
    if (autoFlag == 1) {
      autoFlag = -1;
    } else autoFlag = 1;
  }
}

void evaluateStringCommand(){
  if (msgBuffer[3] == ATR_MSG_ECHO)   echoCommand();
  else if (msgBuffer[3] == ATR_STATUS)  statusCommand();
  else if (msgBuffer[3] == ATR_SET_MOTOR) setMotorPower();

}
void evaluateBinaryCommand(){
  Serial.print("N/A-");
  echoCommand();
}

void echoCommand(){
  Serial.print("[");
  for(int i = 0; i <= msgBufferPointer; i++){
    Serial.print(msgBuffer[i]);
  }
  Serial.println("]");
}

void statusCommand(){
  Serial.print("auto flag: ");
  Serial.println(autoFlag); 
  Serial.print("speed R: "); 
  Serial.println(mSpeedR);
  Serial.print("speed L: "); 
  Serial.println(mSpeedL); 
  Serial.print("odometer R: ");
  Serial.println(odometerR); 
  Serial.print("odometer L: "); 
  Serial.println(odometerL); 
  Serial.print("line R: "); 
  Serial.println(lineR); 
  Serial.print("line L"); 
  Serial.println(lineL); 
  Serial.print("sonar: ");  
  Serial.println(sonar); 
  Serial.print(""); 
}

void setMotorPower(){
  char *p = msgBuffer;
  String str;
  int cnt = 0;
  while ((str = strtok_r(p, " ", &p)) != NULL) { // delimiter is the space
    if (cnt == 2) mSpeedR = map( str.toInt(), 0 , 1024, -255, 255);
    if (cnt == 3) mSpeedL = map( str.toInt(), 0 , 1024, -255, 255);
    if (cnt == 4) autoFlag = str.toInt(); 
    cnt++;
  }
  statusCommand();
}


