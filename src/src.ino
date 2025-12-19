#include "MainStateMachine.h"
#include "MainStateMachine.h"
#include "InputManager.h"
#include "Drivers/LineSensor.h"
#include "Drivers/DistanceSensor.h"
#include "Drivers/MotorDriver.h"
#include "Drivers/SerialDriver.h"
#include "Drivers/BluetoothDriver.h"
#include "Drivers/PortExpander.h"
#include "States/Autonomous.h"
#include "States/Idle.h"
#include "States/RemoteControl.h"
#include "States/Slave.h"
#include "Util.h"
#include "fuck-arduino-kekw/Arduino-LiquidCrystal-I2C-library/LiquidCrystal_I2C.h"
#include "fuck-arduino-kekw/Wire.h"
#include "fuck-arduino-kekw/SoftwareSerial.h"

//LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    //distanceSensorInit();
    serialInit(CALC_BAUD(9600));
    timerZeroInit();
    bluetoothInit();

    Wire.begin();
    portExpanderInit();

    lineSensorInit();
    motorDriverInit();
    
    registerNewState(Idle, &idleState);
    registerNewState(Slave, &slaveState);
    registerNewState(RemoteControl, &remoteControlState);
    registerNewState(Autonomous, &autonomousState); 
}

void loop() {
    //run all of the update functions
    portExpanderUpdate();
    lineSensorUpdate();
    bluetoothUpdate();
    inputManagerUpdate();

    //execute the current state
    runCurrentState();
}

int main(void) {
    setup();
    while(1) { loop(); }
}
