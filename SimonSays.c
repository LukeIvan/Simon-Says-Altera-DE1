// "Simon Says" Written By Luke Ivan, 3rd Year Computer Engineering

#define PUSHBUTTON_DATA (volatile unsigned int*) 0xFF200050
#define PUSHBUTTON_MASK (volatile unsigned int*) 0xFF200058
#define PUSHBUTTON_EDGE (volatile unsigned int*) 0xFF20005C

#define NIOS2_WRITE_STATUS(value)  (__builtin_wrctl (0, value))
#define NIOS2_READ_IENABLE()	   (__builtin_rdctl (3))
#define NIOS2_WRITE_IENABLE(value) (__builtin_wrctl (3, value))
#define NIOS2_READ_IPENDING()	   (__builtin_rdctl (4))
	
#define LEDS ((volatile unsigned int*) 0xFF200000)

#define TIMER_STATUS (volatile unsigned int*) 0xFF202000
#define TIMER_CONTROL (volatile unsigned int*) 0xFF202004
#define TIMER_HI (volatile unsigned int*) 0xFF20200C
#define TIMER_LO (volatile unsigned int*) 0xFF202008

//Interval timer is interrupt level 0
//Pushbuttons are interrupt level 1
#define TIMER_IPENDING_BIT 0x1
#define PUSHBUTTON_BIT 0x2

/* The assembly language code below handles processor reset */
void the_reset (void) __attribute__ ((section (".reset")));
void the_reset (void)

/*****************************************************************************
 * Reset code. By giving the code a section attribute with the name ".reset" *
 * we allow the linker program to locate this code at the proper reset vector*
 * address. This code jumps to main().                                       *
 *****************************************************************************/

{
  asm (".set noat");         /* the .set commands are included to prevent */
  asm (".set nobreak");      /* warning messages from the assembler */
  asm ("movia r2, main");  /* jump to the C language _startup_ code */
  asm ("jmp r2");            /* (_not_ main, as in the original Altera file) */
}

/* The assembly language code below handles exception processing. This
 * code should not be modified; instead, the C language code in the normal
 * function interrupt_handler() can be modified as needed for a given application.
 */

void the_exception (void) __attribute__ ((section (".exceptions")));

/*****************************************************************************
 * Exceptions code. By giving the code a section attribute with the name     *
 * ".exceptions" we allow the linker program to locate this code at the      *
 * proper exceptions vector address. This code calls the interrupt handler   *
 * and later returns from the exception.                                     *
 *****************************************************************************/

void the_exception (void)
{
  asm (".set noat");         /* the .set commands are included to prevent */
  asm (".set nobreak");      /* warning messages from the assembler */
  asm ("subi sp, sp, 128");
  asm ("stw  et, 96(sp)");
  asm ("rdctl et, ct14"); 
  asm ("beq  et, r0, SKIP_EA_DEC");   
  asm ("subi ea, ea, 4");             
  asm ("SKIP_EA_DEC:");
  asm ("stw	r1,  4(sp)"); /* Save all registers */
  asm ("stw	r2,  8(sp)");
  asm ("stw	r3,  12(sp)");
  asm ("stw	r4,  16(sp)");
  asm ("stw	r5,  20(sp)");
  asm ("stw	r6,  24(sp)");
  asm ("stw	r7,  28(sp)");
  asm ("stw	r8,  32(sp)");
  asm ("stw	r9,  36(sp)");
  asm ("stw	r10, 40(sp)");
  asm ("stw	r11, 44(sp)");
  asm ("stw	r12, 48(sp)");
  asm ("stw	r13, 52(sp)");
  asm ("stw	r14, 56(sp)");
  asm ("stw	r15, 60(sp)");
  asm ("stw	r16, 64(sp)");
  asm ("stw	r17, 68(sp)");
  asm ("stw	r18, 72(sp)");
  asm ("stw	r19, 76(sp)");
  asm ("stw	r20, 80(sp)");
  asm ("stw	r21, 84(sp)");
  asm ("stw	r22, 88(sp)");
  asm ("stw	r23, 92(sp)");
  asm ("stw	r25, 100(sp)"); // r25 = bt (r24 = et, because it is saved above) 
  asm ("stw	r26, 104(sp)"); // r26 = gp
  // skip saving r27 because it is sp, and there is no point in saving this
  asm ("stw	r28, 112(sp)"); // r28 = fp 
  asm ("stw	r29, 116(sp)"); // r29 = ea 
  asm ("stw	r30, 120(sp)"); // r30 = ba 
  asm ("stw	r31, 124(sp)"); // r31 = ra 
  asm ("addi	fp,  sp, 128"); 

  asm ("call	interrupt_handler"); // Call C Interrupt Function

  asm ("ldw	r1,  4(sp)"); // Restore all registers 
  asm ("ldw	r2,  8(sp)");
  asm ("ldw	r3,  12(sp)");
  asm ("ldw	r4,  16(sp)");
  asm ("ldw	r5,  20(sp)");
  asm ("ldw	r6,  24(sp)");
  asm ("ldw	r7,  28(sp)");
  asm ("ldw	r8,  32(sp)");
  asm ("ldw	r9,  36(sp)");
  asm ("ldw	r10, 40(sp)");
  asm ("ldw	r11, 44(sp)");
  asm ("ldw	r12, 48(sp)");
  asm ("ldw	r13, 52(sp)");
  asm ("ldw	r14, 56(sp)");
  asm ("ldw	r15, 60(sp)");
  asm ("ldw	r16, 64(sp)");
  asm ("ldw	r17, 68(sp)");
  asm ("ldw	r18, 72(sp)");
  asm ("ldw	r19, 76(sp)");
  asm ("ldw	r20, 80(sp)");
  asm ("ldw	r21, 84(sp)");
  asm ("ldw	r22, 88(sp)");
  asm ("ldw	r23, 92(sp)");
  asm ("ldw	r24, 96(sp)");
  asm ("ldw	r25, 100(sp)");
  asm ("ldw	r26, 104(sp)");
  // skip r27 because it is sp 
  asm ("ldw	r28, 112(sp)");
  asm ("ldw	r29, 116(sp)");
  asm ("ldw	r30, 120(sp)");
  asm ("ldw	r31, 124(sp)");

  asm ("addi	sp,  sp, 128");

  asm ("eret"); // return from exception 
}

//Timer has 100Mhz clock, looking for interrupts every 100ms to act as a random variable
void Init(void){
    *TIMER_LO = 10000000 & 0xFFFF;
    *TIMER_HI = 10000000 & (0xFFFF << 16);
    *TIMER_STATUS = 0;
    *TIMER_CONTROL = 0x7;

    *(PUSHBUTTON_DATA + 2) = 0x3; // Enable interrupts for pushbuttons

    NIOS2_WRITE_STATUS(0x1); //Enable processor interrupts
    NIOS2_WRITE_IENABLE(TIMER_IPENDING_BIT | PUSHBUTTON_BIT);
}

unsigned int random_flag = 1;
unsigned int global_game_flag;
unsigned int buttonPressed;
unsigned int timerRepo; // Repurpose timer for use in playback of LED pattern
unsigned int t_count = 0;
unsigned int gameOverFlag;


void interrupt_handler(void){
    unsigned int ipending = NIOS2_READ_IPENDING();
    if((ipending & TIMER_IPENDING_BIT) == TIMER_IPENDING_BIT){
        *TIMER_STATUS = *TIMER_STATUS & 0x2;
        handleTimer();
    }
    if(((ipending & PUSHBUTTON_BIT) == PUSHBUTTON_BIT) && global_game_flag == 1){
        handlePBin();
    }
}

void patternOccuring(int a){
    global_game_flag == a;
}

void handleTimer(void){
    if(random_flag == 4) random_flag = 1; 
    if(random_flag == 3) random_flag = 4;
    if(random_flag == 2) random_flag = 3;
    if(random_flag == 1) random_flag = 2;
    if(timerRepo = 1){
        t_count++;
        if(t_count == 3) t_count = 0;
    }
    }

void handlePBin(void){
    int press;
    press = *(PUSHBUTTON_DATA+3);
    do
    {
        *LEDS == press; // ensuring LED flashes while button is pressed :)
    } while (*(PUSHBUTTON_DATA+3) > 0);
    
    *(PUSHBUTTON_DATA+3) = press;
    if(press & 0x1 == press){ 
        buttonPressed = 1; 
    } //Key 1
    if (press & 0x2 == press) { 
        buttonPressed = 2; 
    } //Key 2
    if (press & 0x4 == press) { 
        buttonPressed = 3; 
    } //Key 3
    if (press & 0x8 == press) {
        buttonPressed = 4; 
    } //Key 4
}

void gameOverAnim(void){
    timerRepo = 1;
    unsigned int on;
    for(int i = 0; i < 10; i++){
        while(t_count < 10){
            if(on) *LEDS = 0b1111111111;
            else if(!on) *LEDS = 0b1111111111;
        }
        if(on) on = 0;
        else on = 1;
    }
    timerRepo = 0;
}

int main(void){
    Init();

    while(1){
        unsigned int* LIST[50]; //Pattern reaches up to 50 outcomes
        unsigned int count = 0;
        for(int i = 0; i < 50; i++) LIST[i] = 0;
        t_count = 0; 
        do{
            //Display Pattern Section
            LIST[count] = random_flag;
            timerRepo = 1;
            for(int i = 0; i < count; i++){
                t_count = 0;
                while(t_count < 3){
                    *LEDS = LIST[i];
                }
            }
            timerRepo = 0;
            
            //Repeat Pattern Section
            global_game_flag = 1;
            for(int i = 0; i < count; i++){
                int key = buttonPressed;
                buttonPressed = 0;
                if(LIST[count] != buttonPressed){
                    gameOverAnim();
                    gameOverFlag = 1;
                }
                while(buttonPressed == 0){}; // wait until another button interrupt returns
                // ^ Might be bad practice, could not find another solution
            }
            global_game_flag = 0;
            count++;
            if(count >= 49){
                gameOverAnim();
                gameOverAnim();
                gameOverFlag = 1;
            }
        }while(gameOverFlag == 0);
    }
    return 0;
}