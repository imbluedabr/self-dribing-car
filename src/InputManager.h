#pragma once
#include "Drivers/BluetoothDriver.h"
#include "Drivers/SerialDriver.h"
#include "Drivers/MotorDriver.h"
#include "MainStateMachine.h"
#include "Util.h"

#define INPUT_WORD_SIZE 16
#define INPUT_MAX_ARGS 2
extern char inputBuffer[INPUT_MAX_ARGS][INPUT_WORD_SIZE];
extern unsigned char argumentCount;
void flushArguments();
int strncmp( const char * s1, const char * s2, unsigned int n );
void inputManagerUpdate();

void prints(const char* str);
int to_int(const char* str);
void to_str(char* str, int number);
void printh(int number);

void BCDConvert(char* str, uint16_t val);

extern const char StringStates[][16];
