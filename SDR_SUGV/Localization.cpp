#include "Arduino.h"
#include "Protocol.h"
#include "Localization.h"
#include "Sensing.h"
#include "SDR_SUGV.h"
#include "Action.h"



float speedRobot = 0; 
int angle = 0;
int positionX = 0; 
int positionY = 0;
int speedR = 0; 
int speedL = 0; 

void updateOdometer(){
  int R = odometerR * 5.1;
  int L = odometerL * 5.1;
  if (R > L) {
    odometerClock = R;
  } 
  else odometerClock = L;
}


int oldOdometerClock = 0;
float oldMillis = 0.0;

void updateSpeedmeter(){
  float distance = odometerClock - oldOdometerClock;
  float time = currentMillis - oldMillis;
  if (distance > 20) { // we've traveled ~20 mm
    speedRobot = (distance / time) * 1000; // mm per second
    oldOdometerClock = odometerClock;
    oldMillis = currentMillis;
  }
}


