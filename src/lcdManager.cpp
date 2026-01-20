#include "lcdManager.h"
#include "InputManager.h"


// Modus aangeven van waar die in zit
// Rij richting
// Hoelang het aan was

LiquidCrystal_I2C lcd(0x27, 16, 2, LCD_5x8DOTS);
const char lcdRichting[][16] = {
  "HALTED",   
  "FRWRD", 
  "BCKWRD",
  "LEFT",     
  "RIGHT",    
  "HRDLEFT",
  "HRDRIGHT"
};


struct Task lcdTriggerTask = {
     .interval = MS_TO_TICKS(1000),
     .lastUpdate = 0,
     .callback = &lcdUpdate
  };


void lcdInit() {
    lcd.preInit();
    lcd.init();
    lcd.backlight();
    registerTask(&lcdTriggerTask); 
    tick_delay (MS_TO_TICKS(1000));

}

void lcdPrint(char* str) {
    while (*str != '\0') {
        lcd.write(*str++); //Modus aangegeven
    }
}

void lcdUpdate (struct Task* myTask) {
    int remainder = TICKS_IN_SECONDS % 3600;
	int minutes = remainder / 60;
	remainder = remainder % 60;
	int hours = TICKS_IN_SECONDS / 3600;
    char lcdBuffer [17] = "xx:xx:xx        ";
    lcd.setCursor(0, 0);
    BCDConvert(lcdBuffer, hours);
    BCDConvert(lcdBuffer + 3, minutes);
	BCDConvert(lcdBuffer + 6, remainder);

    const char *str = lcdRichting[currentMotorMode];
    memcpy(lcdBuffer + 9, (char*)str, strnlen(str, 16));
    lcdPrint(lcdBuffer);

    lcd.setCursor(0, 1);
    str = StringStates[currentState.id];
	char stateBuffer[17] = "                ";
    memcpy(&stateBuffer, (char*) str, strnlen(str, 16));
    lcdPrint(stateBuffer);

}

