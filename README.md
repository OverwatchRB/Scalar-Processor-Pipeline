# ------------------------ SCALAR PIPELINED PROCESSOR ----------------------------- #

TEAM MEMBERS : 
1) NAME - ROHAN BAGATI, ROLL NUMBER - CS22B082
2) NAME - NEELAVO SEN, ROLL NUMBER - CS22B095

In this lab session, we endeavored to grasp the intricacies of a scalar pipelined processor through software-level simulations, leveraging the C++ programming language for implementation.

# ------------------------------- HOW TO RUN --------------------------------------- #

1. Compile the cpp file LAB8 using:
        `g++ main.cpp`
2. Use the command : `./a.exe`
    (output folders and input folders should be present in the directory before using this command)

This is for Windows, commands for linux would have  `./a.out` instead of `./a.exe`.

# ----------------------------- BRIEF CODE DESCRIPTION ---------------------------- #

# TAKING INPUTS AND VARIABLE DEFINITON #

We have made 3 input arrays - ins[257], data[257] and reg[17] of type string, unsigned int and unsigned int respectively.
We have an "unsigned int pc" variable which traverses the instruction array and acts as the Program Counter.
There is one count array creg[17] to check which registers are undergoing an instruction i.e. are to be written back in the Write-Back section (Data Stalls). This takes care of DATA HAZARDS.
Then we have defnied some variables of type "uint8_t" for destination registers, source registers and immediate values. Also some string variables to carry the instruction forward so that it doesn't get lost.
Also we have defined some variables for Output.txt's output format i.e no. of instructions, cycles per instruction,etc.
We have defined a flag variable for CONTROL HAZARDS. It sets to 1 once JMP or BEQZ is encountered and set back to 0 once PC is updated (Control Stalls)
We have also defined a queue "ALU_OP" which acts as a buffer and stores the execution product in the execution stage and pops it out whenever it's written back.

# PROGRAM EXECUTION #

Our code executes the program in a scalar pipelining fashion in which all 5 things - write back, memory operation, execution, instruction decoding and instruction fetching - are happening in 1 cycle for different instructions.
For e.g. When 1st instruction is getting written back, 2nd would be in memory operation stage, 3rd in execution, 4th in decoding and 5th one in fetching. 
Data Stalls - When a data hazard is encountered, the "creg" array takes care of it and stalls it for an appropriate amount of cycles such that no overwriting occurs and program runs smoothly.
Control Stalls - Our variable "flag" takes care of the stalling whenever JMP or BEQZ occurs. It stalls until PC is updated.
Our variables for Output.txt are also getting updated whenever required.

# -------------------------------------- END ------------------------------------------ #