#pragma once
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "PortExpander.h"

//motor A is the left channel and motor B is the right channel

//port B
#define MOTOR_A_PWM (1<<1)
#define MOTOR_B_PWM (1<<2)

//port expander
#define MOTOR_A_IN1 (1<<4)
#define MOTOR_A_IN2 (1<<5)
#define MOTOR_B_IN3 (1<<6)
#define MOTOR_B_IN4 (1<<7)

enum MotorState {
  HALTED,   //this state turns everything off
  FORWARDS, 
  BACKWARDS,
  LEFT,     //this state does a gradual left turn
  RIGHT,    //this does a gradual right turn
  HARD_LEFT,//this does an in place turn
  HARD_RIGHT
};

//the current motor direction
extern enum MotorState currentMotorState;

//set the absolute speed of the motors
void setMotorSpeed(unsigned char speed);

//chang the speed by a relative amount
void changeMotorSpeed(unsigned char speed);

//change the turning factor and update pwm
void setMotorTurningFactor(float newTurnFactor);

//set the direction of the car
void setMotorState(enum MotorState newMotorState);
void motorDriverInit();

