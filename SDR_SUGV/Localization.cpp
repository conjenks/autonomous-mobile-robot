#include "Localization.h"
#include "Sensing.h"
#include "SDR_SUGV.h"


int speedRobot = 0; 
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
  } else odometerClock = L;
}


int oldOdometerL = 0;
int oldOdometerR = 0;

void updateSpeedmeter(){
  int distance;
  if (odometerL > odometerR) {
   distance = odometerL - oldOdometerL; 
  } else {
   distance = odometerR - oldOdometerR; 
  }
  int time = currentMillis - previousMillis;
  speedRobot = distance / time;
  oldOdometerL = odometerL;
  oldOdometerR = odometerR;
}
