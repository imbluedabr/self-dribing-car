#include "InputManager.h"
#include "Drivers/PortExpander.h"

void prints(const char* str) {
    for (;*str != '\0'; str++) {
        serialTransmit(*str);
    }
}

int strncmp( const char * s1, const char * s2, unsigned int n )
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

//turn number into string
void to_str(char* str, int number) {
    
    static const char lookup[] = "0123456789ABCDEF";

    int i = 12;
    while(i >= 0) {
        *(str++) = lookup[(number >> i) & 0xF];
        i -= 4;
    }
}

void printh(int number) {
    char buff[5];
    buff[4] = '\0';
    to_str((char*) &buff, number);
    prints(buff);
}


//turn string into number
int to_int(const char* str) {
    
    int num = 0;
    char c;
    while (*str != '\0') {
        c = *(str++);
        if (c > 64) {
            c -= 55;
        } else if (c > 47) {
            c -= 48;
        } else {
            
            break;
        }
        num = (num << 4) | c;
    }
    return num;
}

char inputBuffer[INPUT_MAX_ARGS][INPUT_WORD_SIZE] = {0};
unsigned char argumentCount = 0;


void parseArguments(char* data, int size) {
    unsigned char argumentBase = 0;

    for (int i = 0; i < size; i++) {
        if (data[i] == ' ') {
            if (argumentCount++ > INPUT_MAX_ARGS) {
                break;
            }
            i++;//skip space
            argumentBase = 0;
        }
        inputBuffer[argumentCount][argumentBase] = data[i];
        if (argumentBase++ > INPUT_WORD_SIZE) {
            if (argumentCount++ > INPUT_MAX_ARGS) {
                break;
            }
            argumentBase = 0;
        }
    }
}

void flushArguments() {
    for (uint8_t i = 0; i < sizeof(inputBuffer); i++) {
        ((char*) inputBuffer)[i] = 0;
    }
    argumentCount = 0;
    currentState.currentEvent = None;
}

const char StringStates[][16] = {"Idle", "Slave", "RemoteControl", "Autonomous"};

void inputManagerUpdate() {
    //make sure that the input is handeled
    if (currentState.currentEvent == InputComplete) {
        return;
    }

    if (serialInputComplete) {
        parseArguments(serialInputBuffer, serialInputSize);
        serialInputSize = 0;
        serialInputComplete = false;
        currentState.currentEvent = InputComplete;
    } else if (bluetoothStringComplete) {
        parseArguments(bluetoothInputString, bluetoothInputSize);
        bluetoothStringComplete = false;
        bluetoothInputSize = 0;
        currentState.currentEvent = InputComplete;
        prints(inputBuffer[0]);
        prints("\r\n");
    }

    if (strncmp(inputBuffer[0], "setS", INPUT_WORD_SIZE) == 0) {
        for (uint8_t i = 0; i < 4; i++) {
            if (strncmp(inputBuffer[1], StringStates[i], INPUT_WORD_SIZE) == 0) {
                prints("matched state\r\n");
                currentState.id = (enum StateID) i;
            }
        }
        flushArguments();
    } else if (strncmp(inputBuffer[0], "getS", INPUT_WORD_SIZE) == 0) {
        prints(StringStates[currentState.id]);
        prints("\r\n");
        flushArguments();
    } else if (strncmp(inputBuffer[0], "setP", INPUT_WORD_SIZE) == 0) {
        uint8_t val = to_int(inputBuffer[1]);
        char buff[] = "val: xxxx\r\n";
        to_str(buff + 5, val);
        prints(buff);
        portExpanderWrite(val);
        flushArguments();
    } else if (strncmp(inputBuffer[0], "getP", INPUT_WORD_SIZE) == 0) {
        printh(portExpanderData);
        prints("\r\n");
        flushArguments();
    } else if (strncmp(inputBuffer[0], "getPM", INPUT_WORD_SIZE) == 0) {
        printh(portExpanderMode);
        prints("\r\n");
        flushArguments();
    } else if (strncmp(inputBuffer[0], "ss", INPUT_WORD_SIZE) == 0) {
        uint8_t val = to_int(inputBuffer[1]);
        char buff[] = "spd: xxxx\r\n";
        to_str(buff + 5, val);
        prints(buff);
        setMotorSpeed(val);
        flushArguments();
    } else if (strncmp(inputBuffer[0], "stf", INPUT_WORD_SIZE) == 0) {
        uint8_t val = to_int(inputBuffer[1]);
        char buff[] = "trn: xxxx\r\n";
        to_str(buff + 5, val);
        prints(buff);
        setMotorTurningFactor(val);
        flushArguments();
    }
}

void BCDConvert(char* str, uint16_t val) {
    
    uint8_t temp = 0x0;
    for (int i = 0; i < 16; i++) {
        // Add 3 to each BCD digit >= 5
        if ((temp & 0xF) >= 5) temp += 0x3;
        if ((temp & 0xF0) >= 0x50) temp += 0x30;


        temp <<= 1;
        temp |= (val >> 15) & 1;
        val <<= 1;

        if (i == 15) {
			*str++ = ((temp & 0xF0) >> 4) + 48;
			*str = (temp & 0xF) + 48;
        }
    }
}
