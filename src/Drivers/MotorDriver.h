#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "PortExpander.h"

//motor A is the left channel and motor B is the right channel

//port B
#define MOTOR_A_PWM (1<<1)
#define MOTOR_B_PWM (1<<2)

enum MotorState : uint8_t {
  ACCEL,
  DECEL,
  SETM
};

enum Direction : uint8_t {
  FORWARD,
  BACKWARD,
  STOPPED
};

#define ERROR_FACTOR 1
#define ERROR_SHUTOFF 10

struct MotorStateMachine {
  uint8_t targetSpeed;
  enum Direction targetDirection;
  uint8_t currentSpeed;
  enum Direction currentDirection;
  enum MotorState currentState;
  uint8_t motorChannel;
};

enum MotorMode : uint8_t {
  HALTED,   //this state turns everything off
  FORWARDS, 
  BACKWARDS,
  LEFT,     //this state does a gradual left turn
  RIGHT,    //this does a gradual right turn
  HARD_LEFT,//this does an in place turn
  HARD_RIGHT
};

//set the absolute speed of the motors
void setMotorSpeed(uint8_t speed);

//chang the speed by a relative amount
void changeMotorSpeed(int8_t speed);

//change the turning factor and update pwm
void setMotorTurningFactor(uint8_t newTurnFactor);

//set the direction of the car
void setMotorMode(enum MotorMode newMotorState);

//initialize the driver
void motorDriverInit();

void motorDriverUpdate();

