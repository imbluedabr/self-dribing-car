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
    char inChar = (char)bluetooth.read();
    if (inChar == '\r') {
      // Set the flag
      bluetoothStringComplete = true;
    }
    prints("newC\r\n");
    if (bluetoothStringComplete == false){// Add it to the inputString
      if (bluetoothInputSize < BLUETOOTH_BUFFER_SIZE){
        bluetoothInputString[bluetoothInputSize++] = inChar;
      } else {
        bluetoothStringComplete = true;
      }
    }
  }
}

