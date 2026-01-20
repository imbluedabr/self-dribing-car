#include "RemoteControl.h"

void remoteControlState(struct State* currState) {

    if (currState->currentEvent == InputComplete) {

        char cmd = inputBuffer[0][0];  // eerste karakter

        switch (cmd) {

            case 'w':
                setMotorMode(FORWARDS);
                break;

            case 'a':
                setMotorMode(LEFT);
                break;

            case 'd':
                setMotorMode(RIGHT);
                break;

            case 's':
                setMotorMode(BACKWARDS);
                break;

            case 'q':
                setMotorMode(BRIGHT);   // als zo heet
                break;

            case 'e':
                setMotorMode(BLEFT);
                break;

            case 'h':
                if (strncmp(inputBuffer[0], "halt", 4) == 0) {
                    setMotorMode(HALTED);
                } else {
                    prints("unknown command\r\n");
                }
                break;

            default:
                prints("unknown command\r\n");
                break;
        }

        flushArguments();
    }
}


