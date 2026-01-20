#include "PortExpander.h"
       
uint8_t portExpanderData;
uint8_t portExpanderMode;
uint16_t lastReadTime;

enum portEvent : uint8_t {
  NONE,
  READ
} portExpanderEvent;

void portExpanderWrite(uint8_t mode) {
  portExpanderMode = mode;
  Wire.beginTransmission(PORT_EXPANDER_ADRES);
  Wire.write(mode);
  Wire.endTransmission();
}

void portExpanderInit() {
  
  //set int pin to input pullup
  DDRD &= ~(PORT_EXPANDER_INT);
  //set debug to output
  DDRD |= PORT_EXPANDER_DEBUG;

  PORTD |= PORT_EXPANDER_INT;

  //initialize state
  portExpanderData = 0;
  portExpanderMode = 255;
  portExpanderEvent = NONE;
  lastReadTime = 0;
}

void portExpanderUpdate() {

  // request updated data from the pcf
  if (!(PIND & PORT_EXPANDER_INT) && (portExpanderEvent == NONE)) {
    Wire.requestFrom(PORT_EXPANDER_ADRES, 1);
    portExpanderEvent = READ;

    PORTD |= PORT_EXPANDER_DEBUG;
    _delay_us(10);
    PORTD &= ~PORT_EXPANDER_DEBUG;
  }

  // store incoming data (MAX 1x per 250 ms)
  if (Wire.available() &&
      (portExpanderEvent == READ) &&
      ((((uint16_t) ticks) - lastReadTime) > READ_INTERVAL)) {

    lastReadTime = ticks;        // klok resetten
    portExpanderData = Wire.read();
    portExpanderEvent = NONE;
  }
}

