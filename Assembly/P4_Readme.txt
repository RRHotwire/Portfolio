Lab Objectives
The objective for this lab is to become familiar with I/O ports on the HCS12 MCU, pushbuttons to receive input from a user, and LEDs to output the results. Additionally, a sub-objective is to also become more familiar with CodeWarrior (CW) IDE and the D-Bug12 interface and commands.

Description
Implement the logic functions listed in the “Work Task”. In this lab, use the PushButtons (PBs) as the input and the least significant (rightmost) LED as the output (see Figure 1).

 
Figure 1: The four PB switches and the LED (box) used for this task. The numbering for the PBs is shown in the box directly below them.
Work Task
You will implement the logic functions listed below. You have to implement them in a subroutine. You have four PBs that you can use as inputs: PB1, PB2, PB3, and PB4 (see Figure 1). You will only use a relevant amount of input PBs (which PBs to use is listed in the truth tables). You have to two options: 1) create a separate CW project for each function or 2) you can use a single CW project and then use one or a combination of the upper four dip switches to switch between which function is active (the second option is more difficult). You must disable the 7-segment display.

 
1.	Sum-of-products
Using the sum-of-products expression, find and code the simplified logic function for Table 1 using the assembly logic instructions.

A (PB4)	B (PB3)	C (PB2)	D (PB1)	F (LED)
0	0	0	0	1
0	0	0	1	1
0	0	1	0	0
0	0	1	1	0
0	1	0	0	0
0	1	0	1	1
0	1	1	0	0
0	1	1	1	0
1	0	0	0	1
1	0	0	1	1
1	0	1	0	0
1	0	1	1	0
1	1	0	0	0
1	1	0	1	0
1	1	1	0	1
1	1	1	1	1
Table 1: Truth table 1.
2.	Product-of-sums
Using the products-of-sums expression, find and code the simplified logic function for Table 2 using the assembly logic instructions.

A (PB4)	B (PB3)	C (PB2)	D (PB1)	F (LED)
0	0	0	0	0
0	0	0	1	1
0	0	1	0	0
0	0	1	1	1
0	1	0	0	0
0	1	0	1	0
0	1	1	0	0
0	1	1	1	0
1	0	0	0	0
1	0	0	1	1
1	0	1	0	0
1	0	1	1	1
1	1	0	0	1
1	1	0	1	1
1	1	1	0	1
1	1	1	1	0
Table 2: Truth table 2.
