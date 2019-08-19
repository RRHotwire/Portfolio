Description
The next set of commands to be discussed will be conditional branch commands. These are the logical flow operations available to you in assembly. All of the conditional branch commands use the CCR to determine what to do. You should remember that after nearly every command, the CCR is updated to reflect the result of the previous command. There are two classes of conditional branch commands: signed and unsigned. The signed conditional branches expect two-complement signed numbers, while the unsigned ones do not. For example, if you wish to compare two numbers and branch if the first is larger than the second, you need to know if you mean positive definite numbers, or positive and negative numbers. 
Suppose accumulator A has $10 and accumulator B has $FF. If both numbers are positive, A contains the decimal value of 16, and B contains 255. If you were to compare these two values, then do a BHI (Branch Higher than, unsigned), the branch would not go because A is smaller than B. However, if you did a BGT (Branch Greater Than, signed), now A still has the value of 10, but B contains -1 so the branch would execute. 
Some conditional branch commands are invariant to signed and unsigned like BNE and BEQ. BNE is Branch Not Equal, and BEQ is Branch if Equal. Both of these look at the Z bit, which is flipped if the result is a zero. These two branch commands will be very common for your use.
You should also notice that there are two commands for each branch logic, Bxx (e.g., BNE) and LBxx (e.g., LBNE). The L stands for “Long”. If you use the short branch commands (i.e., no “L”), you can only jump from -128 to 127 addresses from your current location. This is because the assembler uses an 8-bit offset for the addressing. If however you use long branch commands (i.e., with a “L” in the beginning), the assembler uses a 16-bit offset, thus you have the ability to jump anywhere in memory.

 
Work Task
You must use the lab template specific to this lab available on TITANium (see “lab_2_template.asm”). Create a program that will copy a block of data from one location to another one byte at a time (i.e., USING A LOOP). The program must follow these requirements:
•	SSTART and DEST are variables that you must use.
•	Stop the transfer when DEST is reached.
•	The program should count the number of negative values that are transferred to the destination. The final count should be stored in a variable NUMNEG.
•	The program should also count the total number of values transferred and put this final count value in a variable TOTAL.
•	The program must contain at least one loop using branch commands.
One thing to note, the example in the lecture on branching copies a block of memory to another block. But in this lab, you have to use the SSTART and DEST variables which are pointing to the address locations shown below. Absolutely no explicit memory addressing (e.g., STAA $1000). You must use variables or registers when possible (e.g., STAA NUMNEG).
