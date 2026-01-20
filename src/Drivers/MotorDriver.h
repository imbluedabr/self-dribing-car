#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "PortExpander.h"
#include "../Util.h"

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

#define MAX_ACCEL 20

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
  FORWARDS, //can do gradual turn by changing turn factor to aw non zero value
  BACKWARDS,
  LEFT,
  RIGHT,
  BLEFT,
  BRIGHT,
  HARD_LEFT,//this does an in place turn
  HARD_RIGHT
};

extern enum MotorMode currentMotorMode;

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

void motorDriverUpdate(struct Task* tsk);

