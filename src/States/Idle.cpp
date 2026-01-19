#include "Idle.h"

void idleState(struct State* currState){
  if (currState->currentEvent == InputComplete) {
    flushArguments();
  }
  setMotorMode(HALTED);
}

