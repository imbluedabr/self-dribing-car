#include "MotorDriver.h"

enum MotorState currentMotorState = HALTED;

float turnFactor = 0;
unsigned char currentMotorASpeed = 0;
unsigned char currentMotorBSpeed = 0;
const int timer1_pwm_freq = 60;

  // Fast PWM, 8-bit, non-inverting, prescaler 64

void timer1Init() {

    TCCR1A = 0;
    TCCR1B = 0;

    // Fast PWM mode 14, non-inverting on A and B
    TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
    TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS10) | (1 << CS11);

    // Set TOP for desired frequency
    ICR1 = (F_CPU / (64 * timer1_pwm_freq)) - 1;

}
void timer1SetPWM() {

    if (currentMotorASpeed == 0) OCR1A = 0;
    else OCR1A = ICR1 / (100 / currentMotorASpeed);

    if (currentMotorBSpeed == 0) OCR1B = 0;
    else OCR1B = ICR1 / (100 / currentMotorBSpeed);
}


static void setMotorParams(bool a_in1, bool a_in2, bool b_in3, bool b_in4) {

  timer1SetPWM();

  if (a_in1 == true) {
    portExpanderMode |= MOTOR_A_IN1;
  } else {
    portExpanderMode &= ~MOTOR_A_IN1;
  }
  if (a_in2 == true) {
    portExpanderMode |= MOTOR_A_IN2;
  } else {
    portExpanderMode &= ~MOTOR_A_IN2;
  }
  if (b_in3 == true) {
    portExpanderMode |= MOTOR_B_IN3;
  } else {
    portExpanderMode &= ~(MOTOR_B_IN3);
  }
  if (b_in4 == true) {
    portExpanderMode |= (MOTOR_B_IN4);
  } else {
    portExpanderMode &= ~(MOTOR_B_IN4);
  }
  portExpanderWrite(portExpanderMode);
}


void setMotorSpeed(unsigned char speed) {
  currentMotorASpeed = speed;
  currentMotorBSpeed = speed;
  timer1SetPWM();
}

void changeMotorSpeed(unsigned char speed) {
  currentMotorASpeed -= speed;
  currentMotorBSpeed -= speed;
  timer1SetPWM();
}

void setMotorTurningFactor(float newTurnFactor) {
  turnFactor = newTurnFactor;
  if (currentMotorState == LEFT) {
    currentMotorASpeed = (unsigned char) currentMotorASpeed*(1 - turnFactor);
    currentMotorBSpeed = (unsigned char) currentMotorBSpeed*(1 + turnFactor);
  } else if (currentMotorState == RIGHT) {
    currentMotorASpeed = (unsigned char) currentMotorASpeed*(1 + turnFactor);
    currentMotorBSpeed = (unsigned char) currentMotorBSpeed*(1 - turnFactor);
  }
  timer1SetPWM();
}

void motorDriverInit(){
  //set the motor pins to ground
  portExpanderWrite(portExpanderMode & ~(MOTOR_A_IN1 | MOTOR_A_IN2 | MOTOR_B_IN3 | MOTOR_B_IN4)); 
  DDRB |= MOTOR_A_PWM | MOTOR_B_PWM;
  timer1Init();
  setMotorSpeed(255);
}

void setMotorState(enum MotorState newMotorState) {
  if (currentMotorState == newMotorState) {
    return;
  }
  currentMotorState = newMotorState;
  switch(newMotorState) { //this can be written a lot better but i dint have the time to make it better
    case HALTED:
      setMotorParams(false, false, false, false);
      break;
    case FORWARDS:
      setMotorParams(true, false, true, false);
      break;
    case BACKWARDS:
      setMotorParams(false, true, false, true);
      break;
    case LEFT:
      currentMotorASpeed = (unsigned char) currentMotorASpeed*(1 - turnFactor);
      currentMotorBSpeed = (unsigned char) currentMotorBSpeed*(1 + turnFactor);
      setMotorParams(true, false, true, false);
      break;
    case RIGHT:
      currentMotorASpeed = (unsigned char) currentMotorASpeed*(1 + turnFactor);
      currentMotorBSpeed = (unsigned char) currentMotorBSpeed*(1 - turnFactor);
      setMotorParams(true, false, true, false);
      break;
    case HARD_LEFT:
      setMotorParams(false, true, true, false);
      break;
    case HARD_RIGHT:
      setMotorParams(true, false, false, true);
      break;
    default:
      break;
  }
}


