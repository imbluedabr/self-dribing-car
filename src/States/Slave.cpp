#include "Slave.h"

void slaveState(struct State* currState){ //TODO: fix this horrible piece of shit
  setMotorSpeed(0x48);
  setMotorTurningFactor(0xFF);
  if (currState->currentEvent == InputComplete) {
    flushArguments();
  }
  
  if (lineSensors[0]) {
    setMotorMode(HARD_RIGHT);
  } else if (lineSensors[3]) {
    setMotorMode(HARD_LEFT);
  } else {
    setMotorMode(FORWARDS);
  }
   
  /* 
  if (middleDistance_cm <20){
    changeMotorSpeed(-10);
  }
  else if (middleDistance_cm >40){
    changeMotorSpeed(10);
  }*/
}


