Lab Objectives
The objective for this lab is to become familiar with interrupts. This will be achieved through the interrupt system on the HCS12, the pushbuttons, and LEDs of the Dragon12+ EVB.

Description
In systems programming, an interrupt is a signal to the processor emitted by hardware or software indicating an event that needs immediate attention. An interrupt alerts the processor to a high-priority condition requiring the interruption of the current code the processor is executing, the current thread. The processor responds by suspending its current activities, saving its state, and executing a small program called an interrupt handler (or interrupt service routine, ISR) to deal with the event. This interruption is temporary, and after the interrupt handler finishes, the processor resumes execution of the previous thread.

Example
Below is an example of how to setup the Dragon12+ EVB to accept an interrupt from PB1 (rightmost pushbutton). The initialization subroutine (INIT) has some new aspects to it. Instructions SEI and CLI will disable and enable maskable interrupts respectively. PIEH is a special port register to enable interrupts on select pins of Port H and PIFH is a special flag register used to acknowledge an interrupt(s). This is done by setting the pin(s) you wish to allow interrupts on to 1 in this register. In this example, I only wish to enable interrupts on pin 0 of Port H. You will also need to define an Interrupt Service Routine (ISR) to handle the interrupt (PB_ISR). In this example, my ISR only loads A with $FF and stores into a global variable called “IN”. Writing 1 to pin 0 of the PIFH register in the ISR is necessary because it acknowledges the interrupt on pin 0 of Port H (i.e., the MOVB command). Without it, the interrupt may not work. You also need to associate the interrupt vector with the ISR as we did in class when we did a SWI interrupt (see the lecture and the “Dragon12+ USB EVB User Manual” on the course website). It’s imperative that this is at the bottom of your code.
 
Code Example
;---------------------------------------------------- 
; Variable/Data Section
; KEEP THIS!!		
;----------------------------------------------------  
          ORG     DATA
IN        FCB     0          
          
;---------------------------------------------------- 
; Code Section
; KEEP THIS!!		
;---------------------------------------------------- 
          ORG     PROG

; Insert your code following the label "Entry"          
Entry:                          ; KEEP THIS LABEL!!
          
          LDS     #PROG
          BSR     INIT
          
MAIN:
          
          ; Do other code
          
          BRA     MAIN
          
;---------------------------------------------------- 
; Init Subroutine
;---------------------------------------------------- 
INIT:
          SEI                   ; Disable maskable interrupts
          
          ; Set Port H, pin 0 (PB1/DIP1)
          BCLR    DDRH, $01
          
          ; Enable interrupts on Port H, pin 0 (PB1/DIP1)
          ; Disable interrupts on Port H for other pins
          MOVB    #$01, PIEH
          MOVB    #$01, PIFH    ; Clear flag for pin 0, set the rest
          
          CLI                   ; Enable maskable interrupts
          
          RTS                             

;---------------------------------------------------- 
; ISR
;----------------------------------------------------          
PB_ISR:
          MOVB    #$01,PIFH	; Acknowledge interrupt
          LDAA    #$FF
          STAA    IN
          RTI
          
;---------------------------------------------------- 
; Interrupt Vector
;----------------------------------------------------
          ; Port H Vector
          ; $3E4C = $FFCC - $C180
          ORG     $3E4C
          FDB     PB_ISR

Work Task
In this lab you will use the PBs to interrupt the MCU and manipulate the LEDs. You will, of course, need to initialize your LEDs and PBs like you did in the previous lab. You must also disable the 7-segment display. You will then need to configure the MCU to accept interrupts from all four pushbuttons (see PIEH register in the example above). You will create an 8-bit global variable called “OUT” that has an initial value of $3C and is repeatedly stored to the LEDs in the main loop. If configured correctly, when a PB is pressed the MCU should execute the ISR. In the ISR, you must determine which button was pressed and perform one of the following operations:

1.	If the PB1 is pressed, then logically shift the contents of “OUT” once to the right
2.	If the PB2 is pressed, then logically shift the contents of “OUT” once to the left
3.	If the PB3 is pressed, then complement (or invert) the value of “OUT”
4.	If the PB4 is pressed, then reset the value of “OUT” back to $3C

Additionally, only one operation should occur per press. If coded correctly, holding the button shouldn’t result in repeatedly performing an operation. Remember to acknowledge the interrupts for pins 0-3 of Port H in your ISR.
