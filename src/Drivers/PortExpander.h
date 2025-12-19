#pragma once

#include "../fuck-arduino-kekw/Wire.h"
#include "../InputManager.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>


#define PORT_EXPANDER_ADRES 0x20
#define PORT_EXPANDER_INT (1 << 2)
#define PORT_EXPANDER_DEBUG (1 << 3)

//this holds the data read from the port expander, this automatically updates using interupts
extern uint8_t portExpanderData;
extern uint8_t portExpanderMode;

//initiate write
void portExpanderWrite(uint8_t mode);

//initialize port expander
void portExpanderInit();

//must be called in the main loop
void portExpanderUpdate();
