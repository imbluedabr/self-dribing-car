#include "Autonomous.h"

void autonomousState(struct State* currState){
  if (middleDistance_cm <30){
    setMotorMode(HARD_LEFT);
  }
  else if (leftDistance_cm <40){
    setMotorMode(RIGHT);
  }
   else if (rightDistance_cm <40){
    setMotorMode(LEFT);
  }
}  

