#include "Util.h"

//tick lenght is 0.000128 seconds
//ticks per time unit is time/0.000128, so ticks per second is 1/0.000128 = 7812

volatile uint64_t ticks;
void timerZeroInit(){
  TCCR0A = 0; // normal mode
  TCCR0B = 0;
  TCCR0B |= 0b00000010; // prescaler 8
  TIMSK0 |= 0b00000001; // enable overflow interrupt
  TCNT0 = 0;
  ticks = 0;
}

ISR(TIMER0_OVF_vect) {
  ticks++;  // One tick per overflow
}

//dont use lol
void tick_delay(uint64_t delay) {
  uint64_t start = ticks;
  while((ticks - start) < delay);
}

struct Task* taskList[TASKLIST_LEN] = {0};

int registerTask(struct Task* task) {
  for (int i = 0; i < TASKLIST_LEN; i++) {
    if (taskList[i] == NULL) {
      taskList[i] = task;
      return i;
    }
  }
  return -1;
}

void taskUpdate() {
  for (int i = 0; i < TASKLIST_LEN; i++) {
    struct Task* t = taskList[i];
    if (t != NULL) {
      if ((ticks - t->lastUpdate) > t->interval) {
        t->callback(t);
        t->lastUpdate = ticks;
      }
    }
  }
}


void eepromWrite(uint16_t address, uint8_t data) {
    // Wait for completion of previous write
    while (EECR & (1 << EEPE));

    // Set up address and data
    EEAR = address;
    EEDR = data;

    // Write logical one to EEMPE
    EECR |= (1 << EEMPE);

    // Start EEPROM write by setting EEPE
    EECR |= (1 << EEPE);
}

uint8_t eepromRead(uint16_t address) {
    // Wait for completion of previous write
    while (EECR & (1 << EEPE));

    // Set up address
    EEAR = address;

    // Start EEPROM read
    EECR |= (1 << EERE);

    // Return data
    return EEDR;
}


uint32_t strnlen(const char* str, uint32_t max_size)
{
    int size = 0;
    while (*str++ != '\0') {
        if (size > max_size) {
            break;
        }
        size++;
    }
    return size;
}


int strncmp( const char * s1, const char * s2, uint32_t n )
{
    while ( n && *s1 && ( *s1 == *s2 ) )
    {
        ++s1;
        ++s2;
        --n;
    }
    if ( n == 0 )
    {
        return 0;
    }
    else
    {
        return ( *(unsigned char *)s1 - *(unsigned char *)s2 );
    }
}

void strncpy(char* dest, char* src, uint32_t size)
{
    while((*src != '\0') && (size-- > 0)) {
        *(dest++) = *(src++);
    }
}


void memset(void* dest, uint32_t size, uint8_t value)
{
    while(size-- > 0) {
        ((uint8_t*) dest)[size] = value;
    }
}

void memcpy(void* dest, void* source, uint32_t size)
{
    while(size-- > 0) {
        ((uint8_t*) dest)[size] = ((uint8_t*) source)[size];
    }
}



