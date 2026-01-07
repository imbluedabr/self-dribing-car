#pragma once
#pragma once

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stddef.h>

#define TICKS_IN_SECONDS (ticks>>13)
#define SEC_TO_TICKS(SEC_T) (SEC_T*7812)
#define MS_TO_TICKS(MS_T) (MS_T*8)

extern volatile uint64_t ticks;

void timerZeroInit();
void timerZeroUpdate();

void tick_delay(uint64_t delay);

//even though its called a task, its more like an interval, like the setInterval function in javascript
struct Task {
    uint32_t interval;
    uint32_t lastUpdate;
    void (*callback)(struct Task*);
};
#define TASKLIST_LEN 16
extern struct Task* taskList[TASKLIST_LEN];

//register a task
int registerTask(struct Task* task);
//run all tasks
void taskUpdate();

void eepromWrite(uint16_t address, uint8_t data);
uint8_t eepromRead(uint16_t address);
