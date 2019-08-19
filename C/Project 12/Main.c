#include <stdint.h>
#include "LCD.h"
#include "SysTick.h"

void main(){

    uint32_t wait_1s = 3000000;
    uint32_t wait_3s = 9000000;
    uint32_t dec = 123456789;


    LCD_Init();


    // A B C
    LCD_OutChar(0x41);
    LCD_OutChar(0x42);
    LCD_OutChar(0x43);
    LCD_OutCmd(0xC0);
    // 1 2 3
    LCD_OutChar(0x31);
    LCD_OutChar(0x32);
    LCD_OutChar(0x33);
    SysTick_Wait(wait_3s);      // Wait 3s


    // Print String
    LCD_Clear();
    LCD_OutString("Hello");
    LCD_OutCmd(0xC0);
    LCD_OutString("World!");
    SysTick_Wait(wait_3s);


    // Print Decimal
    LCD_Clear();
    LCD_OutUDec(dec);
    SysTick_Wait(wait_3s);      // Wait 3s


    // Print Decimal Fixed
    LCD_Clear();
    LCD_OutUFix(10000);
    SysTick_Wait(wait_1s);      // Wait 1s
    LCD_Clear();
    LCD_OutUFix(0);
    SysTick_Wait(wait_1s);      // Wait 1s
    LCD_Clear();
    LCD_OutUFix(1);
    SysTick_Wait(wait_1s);      // Wait 1s
    LCD_Clear();
    LCD_OutUFix(999);
    SysTick_Wait(wait_1s);      // Wait 1s
    LCD_Clear();
    LCD_OutUFix(3333);
    SysTick_Wait(wait_1s);      // Wait 1s
    LCD_Clear();
    LCD_OutUFix(6666);
    SysTick_Wait(wait_1s);      // Wait 1s
    LCD_Clear();
    LCD_OutUFix(9999);

    while(1);                   // Main loop
}
