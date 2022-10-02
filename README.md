# MIPS-Processor
C++ Implementation of a 5 stage pipelined MIPS processor

This work was completed as part of University of Southampton course and any attempts to copy this code for coursework submission may result in Academic Integrity violations.

The file assemblyinstructions.txt holds code to calculate the squares of all numbers from 1-200 in as few instructions as possible.
DataMemory.txt holds the output of the calculations.

The pipelined sequence is as follows:
Instruction Fetch (IF) -> Instruction Decode (ID) -> Execute (EX) -> Memory Access (MEM) (Not always Used) -> Write Back (WB)

In the cases where MEM is not used, the current instruction at MEM skips that step and idles for 1 cycle.

Control, Data and Branch hazards have been implemented.
