#pragma once

#include "fuck-arduino-kekw/Arduino-LiquidCrystal-I2C-library/LiquidCrystal_I2C.h"
#include "MainStateMachine.h"
#include "Util.h"
#include "Drivers/MotorDriver.h"

#define EEPROM_TIME_ADRES 0

void lcdInit();
void lcdUpdate (struct Task* myTask);

void lcdStoreTime();

