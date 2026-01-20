#include "SerialDriver.h"

bool serialInputComplete = false;
int serialInputSize = 0;
char serialInputBuffer[SERIAL_RX_BUFFER_SIZE] = {0};
 
static char serialOutputBuffer[SERIAL_TX_BUFFER_SIZE];
static volatile unsigned char serialTxHead;
static volatile unsigned char serialTxTail; 


//shouldnt this be atomic??
ISR(USART_RX_vect) {

  char inChar = UDR0;

  if (inChar == '\r') {
    serialTransmit('\r');
    serialTransmit('\n');
    serialInputComplete = true;
  } else {
    serialTransmit(inChar);
  }

  if (serialInputComplete == false) {
    serialInputBuffer[serialInputSize] = inChar;
    serialInputSize = (serialInputSize + 1) & SERIAL_RX_BUFFER_MASK;
  }
}

ISR(USART_UDRE_vect) {
    unsigned char tmptail;
   
    // Cheqck if all data is transmitted
    if (serialTxHead != serialTxTail) {
      // Calculate buffer index
      tmptail = (serialTxTail + 1) & SERIAL_TX_BUFFER_MASK;
   
      // Store new index
      serialTxTail = tmptail;
   
      // Start transmission
      UDR0 = serialOutputBuffer[tmptail];
    } else {
      // Disable UDRE interrupt
      UCSR0B &= ~(1 << UDRIE0);
    }
}

void serialTransmit(char data) {
    unsigned char tmphead;
   
    // Calculate buffer index
    tmphead = (serialTxHead + 1) & SERIAL_TX_BUFFER_MASK;
   
    // Wait for free space in buffer
    while (tmphead == serialTxTail);
   
    // Store data in buffer
    serialOutputBuffer[tmphead] = data;
   
    // Store new index
    serialTxHead = tmphead;
   
    // Enable UDRE interrupt
    UCSR0B |= (1 << UDRIE0);
}

void serialInit(uint16_t ubrr) {
    //initialize variables
    serialTxHead = 0;
    serialTxTail = 0;

    // Set baud rate
    UBRR0H = (unsigned char)(ubrr >> 8);
    UBRR0L = (unsigned char)ubrr;

    // Enable transmitter and reciever
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    sei();
}


