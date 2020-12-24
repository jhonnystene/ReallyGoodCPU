/*
 * RGCPU Emulator
 * Copyright (C) 2020 Johnny Stene

 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * 
 * CPU v1 Design:
 * 4 general-purpose registers
 * Can add, subtract, multiply, divide, XOR, OR, and AND
 * 2K RAM, 512 byte stack, 2k ROM
 */

#include <stdio.h>
#include <stdlib.h>

#define STACK_SIZE 512
#define RAM_SIZE 2048
#define ROM_SIZE 2048

// Init registers
int registers[4] = {0, 0, 0, 0};

int *stack;
int *memory;
int *rom;

// Get value of a register by register ID
int getRegisterValue(int registerId) {
	if(registerId > 3) return 0;
	return registers[registerId];
}

// Return either value passed or value in a register
int getRealValue(int value, int fromReg) {
	int realValue;
	if(fromReg) realValue = getRegisterValue(value);
	else realValue = value;
	return realValue;
}

// Move a value into a register
// registerId: The register to move a value into
// value: The value
// fromReg: Whether or not to treat value as register id
void MOV(int registerId, int value, int fromReg) {
	registers[registerId] = getRealValue(value, fromReg);
}

// Add a value to a register
// registerId: The register to add to
// value: The value
// fromReg: If we're getting from a register
void ADD(int registerId, int value, int fromReg) {
	registers[registerId] += getRealValue(value, fromReg);
}

// Subtract a value from a register
// registerId: The register to subtract from
// value: The value
// fromReg: If we're getting from a register
void SUB(int registerId, int value, int fromReg) {
	registers[registerId] -= getRealValue(value, fromReg);
}

// Multiply a register with a value
// registerId: The register to multiply
// value: The value
// fromReg: If we're getting from a register
void MUL(int registerId, int value, int fromReg) {
	registers[registerId] *= getRealValue(value, fromReg);
}

// Divide a register by a value
// registerId: The register to divide
// value: The value
// fromReg: If we're getting from a register
void DIV(int registerId, int value, int fromReg) {
	registers[registerId] /= getRealValue(value, fromReg);
}

// XOR a register by a value
// registerId: The register
// value: The value
// fromReg: If we're getting from a register
void XOR(int registerId, int value, int fromReg) {
	registers[registerId] = registers[registerId] ^ getRealValue(value, fromReg);
}

// OR a register by a value
// registerId: The register
// value: The value
// fromReg: If we're getting from a register
void OR(int registerId, int value, int fromReg) {
	registers[registerId] = registers[registerId] | getRealValue(value, fromReg);
}

// AND a register by a value
// registerId: The register
// value: The value
// fromReg: If we're getting from a register
void AND(int registerId, int value, int fromReg) {
	registers[registerId] = registers[registerId] & getRealValue(value, fromReg);
}

// Push a value onto the stack
void PUSH(int value, int fromReg) {
	stack ++;
	stack[0] = getRealValue(value, fromReg);
}

// Pop a value from the stack
void POP(int registerId) {
	registers[registerId] = stack[0];
	stack --;
}

void STO(int addr, int value, int fromReg) {
	memory[addr] = getRealValue(value, fromReg);
}

void GET(int addr, int registerId) {
	registers[registerId] = memory[addr];
}

void runCode(int* code) {
	/*
	 * Instructions added in RGCPU v1:
	 * 0x00 - NOP
	 * 0x01 - MOV
	 * 0x02 - ADD
	 * 0x03 - SUB
	 * 0x04 - MUL
	 * 0x05 - DIV
	 * 0x06 - XOR
	 * 0x07 - OR
	 * 0x08 - AND
	 * 0x09 - HLT
	 * 0x0A - PUSH
	 * 0x0B - POP
	 * 0x0C - STO
	 * 0x0D - GET
	 * 
	 * 0xFF - END
	 */
	 
	 int instruction;
	 while(instruction != 0xFF) {
		 instruction = code[0];
		 int arg0 = code[1];
		 int arg1 = code[2];
		 int arg2 = code[3];
		 code += 4;
		 if(instruction == 0x00) {}
		 else if(instruction == 0x01) MOV(arg0, arg1, arg2);
		 else if(instruction == 0x02) ADD(arg0, arg1, arg2);
		 else if(instruction == 0x03) SUB(arg0, arg1, arg2);
		 else if(instruction == 0x04) MUL(arg0, arg1, arg2);
		 else if(instruction == 0x05) DIV(arg0, arg1, arg2);
		 else if(instruction == 0x06) XOR(arg0, arg1, arg2);
		 else if(instruction == 0x07) OR(arg0, arg1, arg2);
		 else if(instruction == 0x08) AND(arg0, arg1, arg2);
		 else if(instruction == 0x09) { while(1) {} }
		 else if(instruction == 0x0A) PUSH(arg0, arg1);
		 else if(instruction == 0x0B) POP(arg0);
		 else if(instruction == 0x0C) STO(arg0, arg1, arg2);
		 else if(instruction == 0x0D) GET(arg0, arg1);
	 }
}

int main(int argc, char *argv[]) {
	// Initialise stack and memory
	stack = malloc(sizeof(int) * STACK_SIZE);
	memory = malloc(sizeof(int) * RAM_SIZE);
	rom = malloc(sizeof(int) * ROM_SIZE);
	
	printf("Really Good CPU Emulator v1.0.0\n");
	printf("By Johnny\n");
	if(argc == 1) {
		printf("Usage: rgcpu <ROM FILE>\n");
		return 1;
	}
	
	FILE *romFile = fopen(argv[1], "rb");
	fread(rom, sizeof(int), ROM_SIZE, romFile);
	
	runCode(rom);
	
	//int code[17] = {0x01, 0x00, 0x04, 0x00, /* */ 0x0A, 0x00, 0x01, 0x00, /* */ 0x05, 0x00, 0x02, 0x00, /* */ 0x0B, 0x00, 0x00, 0x00, /* */ 0xFF};
	//runCode(&code[0]);
	//printf("%d", registers[0]); 
}
