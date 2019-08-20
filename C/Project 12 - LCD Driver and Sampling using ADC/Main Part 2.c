#include <stdint.h>
#include "SysTickInts.h"
#include "LCD.h"
#include "msp432p401r.h"
#include "ADC14.h"


void main() {

    LCD_Init();

    while(1){
        LCD_Clear();
        LCD_OutString("EGCP 450 Lab 7");
        LCD_OutCmd(0xC0);
        LCD_OutUFix(SysTick_Mailbox());
        LCD_OutString(" cm");
        SysTick_Wait(1000000);
    }
}