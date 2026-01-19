#include "Slave.h"

void slaveState(struct State* currState){ //TODO: fix this horrible piece of shit
  if (!lineSensors[0] && !lineSensors[1] && !lineSensors[2] && !lineSensors[3] ){
    setMotorMode(FORWARDS);
  }
  else if (
      (lineSensors[0] && (lineSensors[2] || lineSensors[3])) ||
      (lineSensors[1] && (lineSensors[2] || lineSensors[3]))
      ) {
	  setMotorMode(FORWARDS);
  }
  else if (lineSensors[1] == 1) {
    setMotorMode(LEFT);
  }
  else if(lineSensors[0] == 1){
    setMotorMode(HARD_LEFT);
  }
  else if(lineSensors[2] == 1){
    setMotorMode(RIGHT);
  }
   else if(lineSensors[3] == 1){
    setMotorMode(HARD_RIGHT);
  } 
 
  if (middleDistance_cm <20){
    changeMotorSpeed(-10);
  }
  else if (middleDistance_cm >40){
    changeMotorSpeed(10);
  }
}


