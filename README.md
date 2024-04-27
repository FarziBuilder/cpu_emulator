# cpu_emulator
27th April:- I can build my own CPUs

# Specs
This is a 8-bit CPU with 64 Kb of addressable memory, emulating 6502 microprocessor (OG used by Woz to make Apple 1).  
This just does basic LDA and JSR instructions of the CPU 
It can also create a subroutine, and branch out to execute other instructions.  

# Some Notes  
Emulating 6502 microprocessor
- zero page :- First 256 bytes of the CPU
- Program Counter;- Holds the memory address to the next line of instructions to be executed
- Stack:- Reusable memory (256 bytes)
- Stack Pointer:- 8 bit register holding the last 8 bits of the next free location of the stack
- Processor Regsiter:- Quick accessible registers for storing imp data
    - Input Register X,Y:- Stores the counter values
    - Accumulator:- Stores the counter values (stores all logical operations , i guess)
- Flags:- 1-bit register values to store the results of execution of an instruction (Carry, etc)

Fns defined
- Reset():- sets the RF, SP values + Sets the vakue of the Registers
- Mem:- Stores
- ReadWord for reading the 2 bytes after address of the opcode
- WriteWord for storing requisite values to the stack pointer
- FetchByte:- For reading a register value and incrementing PC. ReadByte:- For just reading and not incrementing

Operators defined:-
- [], used for accessing the memory class and changing the Data var inside it.

In JSR, after address at which opcode is stored, the next 2 bytes are the addresses of the instructions of subroutine.

---
64 Kb addressable memory, 8-bit registers microprocessor
Can't really do much except find specific memory locations, understand what to do with the upcoming registers, do those basic stuff.
For making it do useful stuff, you need other I/O devices like screens, keyboards whose register values it will change to make them do useful stuff (like print hello)
For making prog languages run on it. Make a compiler which can convert high-level prog to low-level instructions which can run on this CPU emulator (need to learn more here!)
