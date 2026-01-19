#include "lcdManager.h"
#include "InputManager.h"


// Modus aangeven van waar die in zit
// Rij richting
// Hoelang het aan was

LiquidCrystal_I2C lcd(0x27, 16, 2);
const char lcdRichting[][16] = {
  "HALTED",   
  "FORWARDS", 
  "BACKWARDS",
  "LEFT",     
  "RIGHT",    
  "HARD_LEFT",
  "HARD_RIGHT"
};


struct Task lcdTriggerTask = {
     .interval = MS_TO_TICKS(1000),
     .lastUpdate = 0,
     .callback = &lcdUpdate
  };


void lcdInit() {
    lcd.init();
    lcd.backlight();
    registerTask(&lcdTriggerTask); 
    tick_delay (MS_TO_TICKS(1000));

}



void lcdUpdate (struct Task* myTask) {
    
    char lcdBuffer [4];
    
    lcd.setCursor(0, 0);

    to_str(lcdBuffer, TICKS_IN_SECONDS);
    for (int i = 0; i < 4 ; i++) {
        lcd.write(lcdBuffer[i]);
    }




    lcd.write(' ');

    const char *str = StringStates[currentState.id];

    while (*str != '\0') {
        lcd.write(*str++); //Modus aangegeven
    }
    lcd.setCursor(0, 1);

    str = lcdRichting[currentMotorMode];

    while (*str != '\0') {
        lcd.write(*str++); //Modus aangegeven
        
    }




    
}

