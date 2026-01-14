#include "MotorDriver.h"

struct MotorStateMachine channelA;
struct MotorStateMachine channelB;
enum MotorMode currentMotorMode;

const enum Direction controlMap[7][2] = {
  {STOPPED, STOPPED},
  {FORWARD, FORWARD},
  {BACKWARD, BACKWARD},
  {FORWARD, FORWARD},
  {FORWARD, FORWARD},
  {BACKWARD, FORWARD},
  {FORWARD, BACKWARD}
};

uint8_t motorBaseSpeed;
uint8_t motorTurningFactor;

const int timer1_pwm_freq = 60;

//fast PWM, 8-bit, non-inverting, prescaler 64
void timer1Init() {

    TCCR1A = 0;
    TCCR1B = 0;

    //set timer mode
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10) | (1 << CS11);

    //set TOP
    ICR1 = (F_CPU / (64 * timer1_pwm_freq)) - 1;

}

void timer1SetPWM(uint8_t currentMotorASpeed, uint8_t currentMotorBSpeed) {

    if (currentMotorASpeed == 0) OCR1A = 0;
    else OCR1A = ICR1 / (100 / currentMotorASpeed);

    if (currentMotorBSpeed == 0) OCR1B = 0;
    else OCR1B = ICR1 / (100 / currentMotorBSpeed);
}


void setMotorParam(uint8_t channel, bool m0, bool m1) {
  portExpanderMode |= (m0 << (channel*2 + 4)) | (m1 << (channel*2 + 5)); //set the bits that we need to set
  portExpanderMode &= ~((m0 << (channel*2 + 4)) | (m1 << (channel*2 + 5)));
  portExpanderWrite(portExpanderMode);
}


void MotorStateAccelerate(struct MotorStateMachine* channel) {
  if (channel->targetDirection != channel->currentDirection) {
    channel->currentState = DECEL;
  } else {
    int16_t error = channel->targetSpeed - channel->currentSpeed;
    channel->currentSpeed += error >> ERROR_FACTOR;
  }
}

void MotorStateDecelerate(struct MotorStateMachine* channel) { //decelerate the motor to 0
  int16_t error = -(channel->currentSpeed);
  channel->currentSpeed += error >> ERROR_FACTOR;
  if (error < ERROR_SHUTOFF) {
    channel->currentState = SETM;
  }
}

void MotorStateSetMotor(struct MotorStateMachine* channel) {
  uint8_t motorChannel = channel->motorChannel;
  if (channel->targetDirection == FORWARD) {
    setMotorParam(motorChannel, true, false);
  } else if (channel->targetDirection == BACKWARD) {
    setMotorParam(motorChannel, false, true);
  } else if (channel->targetDirection == STOPPED) {
    setMotorParam(motorChannel, false, false);
    channel->targetSpeed = 0;
  }
  channel->currentState = ACCEL;
}

void MotorStateUpdate(struct MotorStateMachine* channel) {
  switch(channel->currentState) {
    case ACCEL:
      MotorStateAccelerate(channel);
      break;
    case DECEL:
      MotorStateDecelerate(channel);
      break;
    case SETM:
      MotorStateSetMotor(channel);
      break;
  }
}

void setMotorSpeed(uint8_t speed) {
  motorBaseSpeed = speed;
  setMotorMode(currentMotorMode); 
}

void changeMotorSpeed(int8_t speed) {
  uint16_t tmp = motorBaseSpeed + speed;
  if (tmp < 0) {
    tmp = 0;
  } else if (tmp > 255) {
    tmp = 255;
  }
  motorBaseSpeed = tmp;
  setMotorMode(currentMotorMode); 
}

void setMotorTurningFactor(uint8_t newTurnFactor) {
  motorTurningFactor = newTurnFactor;
  setMotorMode(currentMotorMode); //update motor channels
}

void setMotorMode(enum MotorMode newMotorMode) {
  channelA.targetDirection = controlMap[newMotorMode][0];
  channelB.targetDirection = controlMap[newMotorMode][1];
  if (newMotorMode == LEFT) {
    channelA.targetSpeed = (motorBaseSpeed * (256 + motorTurningFactor)) >> 8;
    channelB.targetSpeed = (motorBaseSpeed * (256 - motorTurningFactor)) >> 8;
  } else if (newMotorMode == RIGHT) {
    channelA.targetSpeed = (motorBaseSpeed * (256 - motorTurningFactor)) >> 8;
    channelB.targetSpeed = (motorBaseSpeed * (256 + motorTurningFactor)) >> 8;
  }
  currentMotorMode = newMotorMode;
}

void motorDriverInit() {
  //set the motor pins to ground
  setMotorParam(0, false, false);
  setMotorParam(1, false, false);

  DDRB |= MOTOR_A_PWM | MOTOR_B_PWM;

  currentMotorMode = HALTED;
  motorBaseSpeed = 100;
  motorTurningFactor = 128; //25%
  timer1Init();
}

void motorDriverUpdate() {
  MotorStateUpdate(&channelA);
  MotorStateUpdate(&channelB);
  timer1SetPWM(channelA.currentSpeed, channelB.currentSpeed);
}

