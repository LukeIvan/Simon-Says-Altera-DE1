#include "nios2_control.h"

#define PUSHBUTTON_MASK (volatile unsigned int*) 0xFF200058
#define PUSHBUTTON_EDGE (volatile unsigned int*) 0xFF20005C

//For the purposes of this experiment, only the edgecapture register is considered. The value held in the pushbutton
//is not necessary, the change in output is. 
	
#define LEDS ((volatile unsigned int*) 0xFF200000)

#define TIMER_STATUS (volatile unsigned int*) 0xFF202000
#define TIMER_CONTROL (volatile unsigned int*) 0xFF202004
#define TIMER_HI (volatile unsigned int*) 0xFF20200C
#define TIMER_LO (volatile unsigned int*) 0xFF202008

//Interval timer is interrupt level 0
//Pushbuttons are interrupt level 1
#define TIMER_IPENDING_BIT 0x1
#define PUSHBUTTON_BIT 0x2


//Timer has 100Mhz clock, looking for interrupts every 10ms to act as a random variable
void Init(void){
    *TIMER_LO = 10000000 & 0xFFFF;
    *TIMER_HI = 10000000 & (0xFFFF << 16);
    *TIMER_STATUS = 0;
    *TIMER_CONTROL = 0x7;

    NIOS2_WRITE_STATUS(0x1); //Enable processor interrupts
    NIOS2_WRITE_IENABLE(TIMER_IPENDING_BIT | PUSHBUTTON_BIT);
}
unsigned int random_flag;

void interrupt_handler(void){
    unsigned int ipending = NIOS2_READ_IPENDING();
    if((ipending & TIMER_IPENDING_BIT) == TIMER_IPENDING_BIT){
        *TIMER_STATUS = *TIMER_STATUS & 0x2;
        handleTimer();
    }
    if((ipending & PUSHBUTTON_BIT) == PUSHBUTTON_BIT){
        handlePBin();
    }
}

void handleTimer(void){

}

void handlePBin(){
    
}