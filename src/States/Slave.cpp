#include "Slave.h"

enum MotorMode lastDirection = FORWARDS;

uint32_t lastSignal = 0;

const enum MotorMode convert[9] = {
  FORWARDS,
  FORWARDS,
  FORWARDS,
  RIGHT,
  LEFT,
  FORWARDS,
  FORWARDS,
  FORWARDS,
  FORWARDS
};

// why, why did i not make a full FSM implementation from the start
void slaveState(struct State* currState) {
  setMotorSpeed(0x48);
  setMotorTurningFactor(0xFF);

  if (currState->currentEvent == InputComplete) { //no event buffer :(
    flushArguments();
  }
  
  if (lineSensors[0]) {
    lastSignal = ticks;
    setMotorMode(LEFT);
    lastDirection = LEFT;

  } else if (lineSensors[2]) {
    lastSignal = ticks;
    setMotorMode(RIGHT);
    lastDirection = RIGHT;

  } else if (lineSensors[1]) {
    lastSignal = ticks;
    setMotorMode(lastDirection);
    prints("lsd\r\n");

  } else {
    if ((ticks - lastSignal) > MS_TO_TICKS(150)) {
      setMotorMode(convert[lastDirection]); //we go back to the last direction that we detected the line
      prints("lsego\r\n");
      lastSignal = ticks;
    }
  }

  /* 
  if (middleDistance_cm <20){
    changeMotorSpeed(-10);
  }
  else if (middleDistance_cm >40){
    changeMotorSpeed(10);
  }*/
}


