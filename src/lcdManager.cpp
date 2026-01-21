#include "lcdManager.h"
#include "InputManager.h"
#include "Util.h"

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
  "BLEFT",
  "BRIGHT",    
  "HRDLEFT",
  "HRDRIGHT"
};


struct Task lcdTriggerTask = {
     .interval = MS_TO_TICKS(1000),
     .lastUpdate = 0,
     .callback = &lcdUpdate
  };


void lcdStoreTime() {
  cli();
  uint8_t* word = (uint8_t*) &ticks;
  for (uint8_t i = 0; i < 8; i++) {
    eepromWrite(EEPROM_TIME_ADRES + i, word[i]);
  }
  sei();
}


void lcdInit() {
  lcd.preInit();
  lcd.init();
  lcd.backlight();
  registerTask(&lcdTriggerTask); 
  tick_delay (MS_TO_TICKS(1000));
  uint8_t word[8];
  for (uint8_t i = 0; i < 8; i++) {
    word[i] = eepromRead(EEPROM_TIME_ADRES + i);
  }
  memcpy((void*) &ticks, word, 8);
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
  char stateBuffer[17] = "          xx%   ";
  memcpy(&stateBuffer, (char*) str, strnlen(str, 16));
  
  float spd = ((float)motorBaseSpeed)/255;
  uint16_t intspd = (uint16_t)(spd*100);
  const char* fullspd = "100%";
  if (intspd == 100) {
    memcpy(stateBuffer + 10, (void*) fullspd, 4);
  } else {
    BCDConvert(stateBuffer + 10, intspd);
  }
  lcdPrint(stateBuffer);
}

