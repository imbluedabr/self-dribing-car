#include "Slave.h"

void slaveState(struct State* currState){ //TODO: fix this horrible piece of shit
  setMotorSpeed(0x48);
  setMotorTurningFactor(0xFF);
  if (currState->currentEvent == InputComplete) {
    flushArguments();
  }
  
  if (lineSensors[0] | lineSensors[1]) {
    setMotorMode(RIGHT);
  } else if (lineSensors[3] | lineSensors[2]) {
    setMotorMode(LEFT);
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


