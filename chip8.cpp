#include "chip8.h"

Chip8::Chip8()
        : pc(0x200u), index(0x0u), sound_timer(0x0u), delay_timer(0x0u), sp(0x0u)
{

        fontset =
	{
		0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
		0x20, 0x60, 0x20, 0x20, 0x70, // 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
		0x90, 0x90, 0xF0, 0x10, 0x10, // 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
		0xF0, 0x10, 0x20, 0x40, 0x40, // 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
		0xF0, 0x90, 0xF0, 0x90, 0x90, // A
		0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
		0xF0, 0x80, 0x80, 0x80, 0xF0, // C
		0xE0, 0x90, 0x90, 0x90, 0xE0, // D
		0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
		0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

}

void Chip8::load_rom(std::string filename)
{


}

void Chip8::cycle()
{


}

//Instructions
         
// 00E0 - CLS
void op_00e0()
{

}

// 00EE - RET
void op_00ee()
{
        
}

// 0nnn - SYS addr
void op_0nnn()
{

}

// 1nnn - JP addr
void op_1nnn(){

}

// 2nnn - CALL addr
void op_2nnn()
{

}

// 3xkk - SE Vx, byte
void op_3xkk()
{

}

// 4xkk - SNE Vx, byte
void op_4xkk()
{

}

// 5xy0 - SE Vx, Vy
void op_5xy0()
{

}

// 6xkk - LD Vx, byte
void op_6xkk()
{

}

// 7xkk - ADD Vx, byte
void op_7xkk()
{

}

// 8xy0 - LD Vx, Vy
void op_8xy0()
{

}

// 8xy1 - OR Vx, Vy
void op_8xy1()
{

}

// 8xy2 - AND Vx, Vy
void op_8xy2()
{

}

// 8xy3 - XOR Vx, Vy
void op_8xy3()
{

}

// 8xy4 - ADD Vx, Vy
void op_8xy4()
{

}

// 8xy5 - SUB Vx, Vy
void op_8xy5()
{

}

// 8xy6 - SHR Vx {, Vy}
void op_8xy6()
{

}

// 8xy7 - SUBN Vx, Vy
void op_8xy7()
{

}

// 8xyE - SHL Vx {, Vy}
void op_8xye()
{

}

// 9xy0 - SNE Vx, Vy
void op_9xy0()
{

}

// Annn - LD I, addr
void op_annn()
{

}

// Bnnn - JP V0, addr
void op_bnnn()
{

}

// Cxkk - RND Vx, byte
void op_cxkk()
{

}

// Dxyn - DRW Vx, Vy, nibble
void op_dxyn()
{

}

// Ex9E - SKP Vx
void op_ex9e()
{

}

// ExA1 - SKNP Vx
void op_exa1()
{

}

// Fx07 - LD Vx, DT
void op_fx07()
{

}

// Fx0A - LD Vx, K
void op_fx0a()
{

}

// Fx15 - LD DT, Vx
void op_fx15()
{

}

// Fx18 - LD ST, Vx
void op_fx18()
{

}

// Fx1E - ADD I, Vx
void op_fx1e()
{

}

// Fx29 - LD F, Vx
void op_fx29()
{

}

// Fx33 - LD B, Vx
void op_fx33()
{

}

// Fx55 - LD [I], Vx
void op_fx55()
{

}

// Fx65 - LD Vx, [I]
void op_fx65()
{

}