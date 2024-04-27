#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;

const unsigned int MAX_MEM = 1024 * 64; //2^16
using Byte = unsigned char;
using Word = unsigned short;

using u32 = unsigned int;

struct Mem {
    Byte Data[MAX_MEM];

    void Initialize() {
        for (u32 i = 0; i < MAX_MEM; i++) {
            Data[i] = 0;
        }
    }

    Byte& operator[](u32 address) {
        return Data[address];
    }

    void WriteWord(
        Word value, 
        Word& Address,
        u32& Cycles
        )
    {
        Data[Address] = value & 0xFF; //PC is a word, so you need to store it this way
        Data[Address + 1] = (value >> 8);
        Address -= 1; //for moving the stack pointer after storing the location. Most prob it needs to be -1 rather than +1.
        Cycles -= 2;
    }
};

struct CPU {
    Word PC;
    Word SP;

    Byte A, X, Y;

    Byte C : 1;
    Byte Z : 1;
    Byte I : 1;
    Byte D : 1;
    Byte B : 1;
    Byte V : 1;
    Byte N : 1;

    static constexpr Byte INS_LDA_IM = 0xA9, 
    INS_LDA_ZP = 0xA5,
    INS_LDA_ZPX = 0xB5,
    INS_JSR = 0xB7; //static constexpr belong to class not the objects, hence can be referenced via CPU::INS_JSR!

    void Reset(Mem& mem) {
        PC = 0xFFFC;
        SP = 0x0100;
        A = X = Y = 0;
        C = Z = I = D = B = V = N = 0;

        mem.Initialize();
    }

    Byte FetchByte(u32& cycle, Mem& memory) {
        Byte Data = memory[PC];
        PC++;
        cycle--;
        return Data;
    }
    Byte ReadByte(u32& cycle,Byte Address, Mem& memory)
    {
        Byte Data = memory[Address];
        cycle--;
        return Data;
    }

    void LDASetStatus()
    {
        N = ((A & 0b10000000) > 0);
        Z = (A == 0);
    }
 
    Word ReadWord(u32& cycle, Mem& memory)
    {
        Word Data = memory[PC];
        PC++;
        Data |= (memory[PC] << 8);
        cycle -= 2;
        return Data;
    }

    

    void Execute(u32 Cycles, Mem& memory) {
        while (Cycles > 0) {
            Byte value = FetchByte(Cycles, memory);
            //cout << value;
            switch (value) 
            {
                case INS_LDA_IM:
                {
                    Byte Value = FetchByte(Cycles, memory);
                    A = Value;
                    LDASetStatus();
                    //cout << "check" <<endl;
                    
                } break;
                case INS_LDA_ZP:
                {
                    Byte Value = FetchByte(Cycles, memory); //so 3 cycles needed to run this instruction. First one has the address which has the opcode stored, 2nd one gives the address where the instruction is stored. 3rd one goes into the address where the final instruction is there, which is stored in accumulator
                    A = ReadByte(Cycles,Value, memory);
                    LDASetStatus();
                    
                } break;
                case INS_LDA_ZPX:
                {
                    Byte Value = FetchByte(Cycles, memory);
                    Value += X;
                    Cycles--;
                    A = ReadByte(Cycles,Value, memory);
                    LDASetStatus();
                    
                } break;
                case INS_JSR:
                {
                    Word instruc = ReadWord(Cycles, memory);
                    memory.WriteWord(
                        PC - 1, SP, Cycles);
                    PC =  instruc;
                    Cycles--;
                    
                } break;
                default:
                    cout << "You are in default " << endl;
                    return;
            }
        }
    }
};

int main() {
    Mem mem;
    CPU cpu;
    cpu.Reset(mem);
    //cpu.Execute(2, mem);


    //Testing LDA_ZP
    // mem[0xFFFC] = CPU::INS_LDA_IM;
    // mem[0xFFFD] = 0x42;
    // cpu.Execute(2, mem);
    // cpu.Reset(mem);
    // mem[0xFFFC] = CPU::INS_LDA_ZP;
    // mem[0xFFFD] = 0x42;
    // mem[0x0042] = 0x84;
    // cpu.Reset(mem);
    
    //Testing INS_JSR
    mem[0xFFFC] = CPU::INS_JSR;
    mem[0xFFFD] = 0x42;
    mem[0xFFFE] = 0x42;
    mem[0x4242] = CPU::INS_LDA_IM;
    mem[0x4243] = 0x84;
    cpu.Execute(8, mem);
    cout << static_cast<unsigned int>(cpu.A);
    // cout << static_cast<unsigned int>(mem[0xFFFC]);
    return 0;
}