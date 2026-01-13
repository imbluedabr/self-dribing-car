#include "Slave.h"

void slaveState(struct State* currState){ //TODO: fix this horrible piece of shit
  if (!lineSensors[0] && !lineSensors[1] && !lineSensors[2] && !lineSensors[3] ){
    setMotorState(FORWARDS);
  }
  else if (
      (linesensor[0] && linesensor[2] || linesensor[3])
      (linesensor[1] && linesensor[2] || linesensor[3])
      ) {
	  setMotorState(FORWARDS);
  }
  else if (lineSensors[1] == 1) {
    setMotorState(LEFT);
  }
  else if(lineSensors[0] == 1){
    setMotorState(HARD_LEFT);
  }
  else if(lineSensors[2] == 1){
    setMotorState(RIGHT);
  }
   else if(lineSensors[3] == 1){
    setMotorState(HARD_RIGHT);
  } 
 
  if (middleDistance_cm <20){
    changeMotorSpeed(-10);
  }
  else if (middleDistance_cm >40){
    changeMotorSpeed(10);
  }
}


