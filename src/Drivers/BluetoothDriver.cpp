#include "BluetoothDriver.h"
char bluetoothInputString[BLUETOOTH_BUFFER_SIZE] = {0};
bool bluetoothStringComplete = false;
int bluetoothInputSize = 0;

SoftwareSerial bluetooth(BRX, BTX);

void bluetoothInit() {
  //Set the baud rate for the SoftwareSerial object
  bluetooth.begin(9600);
}

void bluetoothUpdate() {
  while (bluetooth.available()) {
    // Get the new byte
    char inChar = bluetooth.read();
    if (inChar == '\r') {  
      // Set the flag
      bluetoothStringComplete = true;
    }
    if (bluetoothStringComplete == false){// Add it to the inputString

      if (bluetoothInputSize < BLUETOOTH_BUFFER_SIZE){
        bluetoothInputString[bluetoothInputSize++] = inChar;
      } else {
        bluetoothStringComplete = true;
      }
    }
  }
  if (strncmp(bluetoothInputString, "OK+CONN", 7) == 0) {
    for (uint8_t i = 0; i < BLUETOOTH_BUFFER_SIZE; i++) {
      bluetoothInputString[i] = '\0';
    }
    prints("ble connected!\r\n");
    bluetoothInputSize = 0;
  }
  if (strncmp(bluetoothInputString, "OK+LOSS", 7) == 0) {
    for (uint8_t i = 0; i < BLUETOOTH_BUFFER_SIZE; i++) {
      bluetoothInputString[i] = '\0';
    }
    prints("ble disconnected!\r\n");
    bluetoothInputSize = 0;
  }
}

