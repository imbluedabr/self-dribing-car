#include "RemoteControl.h"

void remoteControlState(struct State* currState){
  if (currState->currentEvent == InputComplete){
    if (strncmp(inputBuffer[0], "w", 1) == 0){
      setMotorMode(FORWARDS);
    } else if (strncmp(inputBuffer[0], "a", 1) == 0) {
      setMotorMode(LEFT);
    } else if (strncmp(inputBuffer[0], "d", 1) == 0) {
      setMotorMode(RIGHT);
    } else if (strncmp(inputBuffer[0], "s", 1) == 0) {
      setMotorMode(BACKWARDS);     
    } else if (strncmp(inputBuffer[0], "halt", 4) == 0){
      setMotorMode(HALTED);
    } else {
      prints("unkown command\r\n");
    }
    flushArguments();
  }
}


