#include "chip8.h"

#include <fstream>
#include <iostream>
#include <bitset>

const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int START_ADDRESS = 0x200;


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
        std::ifstream file(filename, std::ios::binary | std::ios::ate);

        if(file.is_open()) {
                std::streampos size = file.tellg();
                char* buffer = new char[size];
                file.seekg(0, std::ios::beg);
                file.read(buffer, size);
                file.close();

                for (long i = 0; i < size; ++i) {
                        memory[START_ADDRESS + i] = buffer[i];
                }

                delete[] buffer;
        }

}

void Chip8::dump_mem()
{
        for (int i{0}; auto &item : memory) {
                std::bitset<8> converted(item);
                std::cout << "Address 0x" << std::hex << i << ": ";
                std::cout << std::hex << converted.to_ulong();
                std::cout << "\n";
                ++i;
        }
}

void Chip8::dump_regs()
{
        std::cout << "\n";
        for (int i{0}; auto &item : v_registers) {
                std::bitset<8> converted(item);
                std::cout << "VRegister " << i << ": ";
                std::cout << converted.to_ulong();
                std::cout << "\n";
                ++i;
        }
}

void Chip8::dump_display()
{
        std::cout << "\n";
        for (int i{0}; auto &pixel : display) {
                std::bitset<8> converted(pixel);
                std::cout << converted.to_ulong();
                if ((i != 0) && (i % 64 == 0)) {
                        std::cout << "\n";
                }
                ++i;
        }
}

void Chip8::cycle()
{
        // Fetch
        uint16_t opcode = (memory[pc] << 8u) | memory[pc + 1];
        // std::cout << "Fetching Op: " << std::hex << opcode << "\n";
        pc += 2;

        // Decode/Execute
        uint16_t addtl_op{};
        uint8_t op = (opcode & 0xf000u) >> 12u;
        uint8_t x = (opcode & 0x0f00u) >> 8u;
        uint8_t y = (opcode & 0x00f0u) >> 4u;
        uint8_t n = opcode & 0x000fu;
        uint8_t kk = opcode & 0x00ffu;
        uint16_t nnn = opcode & 0x0fffu;
        if (op == 0x0) {
                addtl_op = opcode & 0xfu;
                if (addtl_op == 0x0) {
                        op_00e0(x, y, n, kk, nnn);
                }
                else {
                        op_00ee(x, y, n, kk, nnn);
                }
        }
        else if (op == 0x1) {
                op_1nnn(x, y, n, kk, nnn);
        }
        else if (op == 0x2) {
                op_2nnn(x, y, n, kk, nnn);
        }
        else if (op == 0x3) {
                op_3xkk(x, y, n, kk, nnn);
        }
        else if (op == 0x4) {
                op_4xkk(x, y, n, kk, nnn);
        }
        else if (op == 0x5) {
                op_5xy0(x, y, n, kk, nnn);
        }
        else if (op == 0x6) {
                op_6xkk(x, y, n, kk, nnn);
        }
        else if (op == 0x7) {
                op_7xkk(x, y, n, kk, nnn);
        }
        else if (op == 0x8) {
                addtl_op = opcode & 0xfu;
                if (addtl_op == 0x0) {
                        op_8xy0(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x1) {
                        op_8xy1(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x2) {
                        op_8xy2(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x3) {
                        op_8xy3(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x4) {
                        op_8xy4(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x5) {
                        op_8xy5(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x6) {
                        op_8xy6(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x7) {
                        op_8xy7(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0xe) {
                        op_8xye(x, y, n, kk, nnn);
                }
        }
        else if (op == 0x9) {
                op_9xy0(x, y, n, kk, nnn);
        }
        else if (op == 0xa) {
                op_annn(x, y, n, kk, nnn);
        }
        else if (op == 0xb) {
                op_bnnn(x, y, n, kk, nnn);
        }
        else if (op == 0xc) {
                op_cxkk(x, y, n, kk, nnn);
        }
        else if (op == 0xd) {
                op_dxyn(x, y, n, kk, nnn);
        }
        else if (op == 0xe) {
                addtl_op = opcode & 0xfu;
                if (addtl_op == 0xe) {
                        op_ex9e(x, y, n, kk, nnn);
                }
                else {
                        op_exa1(x, y, n, kk, nnn);
                }
        }
        else if (op == 0xf) {
                addtl_op = opcode & 0xffu;
                if (addtl_op == 0x07) {
                        op_fx07(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x0a) {
                        op_fx0a(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x15) {
                        op_fx15(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x18) {
                        op_fx18(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x1e) {
                        op_fx1e(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x29) {
                        op_fx29(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x33) {
                        op_fx33(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x55) {
                        op_fx55(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x65) {
                        op_fx65(x, y, n, kk, nnn);
                }
        }

	if (delay_timer > 0) {
		--delay_timer;
	}

	if (sound_timer > 0) {
		--sound_timer;
	}
}

//Instructions

// NULL         
void Chip8::op_null(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{

}       

// 00E0 - CLS
void Chip8::op_00e0(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        for (auto &value : display) {
                value = 0;
        }
}

// 00EE - RET
void Chip8::op_00ee(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        --sp;
	pc = stack[sp];
}

// 0nnn - SYS addr
void Chip8::op_0nnn(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{

}

// 1nnn - JP addr
void Chip8::op_1nnn(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn){
        pc = nnn;
}

// 2nnn - CALL addr
void Chip8::op_2nnn(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        stack[sp] = pc;
	++sp;
	pc = nnn;
}

// 3xkk - SE Vx, byte
void Chip8::op_3xkk(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        if (v_registers[x] == kk) pc += 2;
}

// 4xkk - SNE Vx, byte
void Chip8::op_4xkk(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        if (v_registers[x] != kk) pc += 2;
}

// 5xy0 - SE Vx, Vy
void Chip8::op_5xy0(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        if (v_registers[x] == v_registers[y]) pc += 2;
}

// 6xkk - LD Vx, byte
void Chip8::op_6xkk(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{       
        v_registers[x] = kk;
}

// 7xkk - ADD Vx, byte
void Chip8::op_7xkk(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        v_registers[x] += kk;
}

// 8xy0 - LD Vx, Vy
void Chip8::op_8xy0(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        v_registers[x] = v_registers[y];
}

// 8xy1 - OR Vx, Vy
void Chip8::op_8xy1(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        v_registers[x] |= v_registers[y];
}

// 8xy2 - AND Vx, Vy
void Chip8::op_8xy2(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        v_registers[x] &= v_registers[y];
}

// 8xy3 - XOR Vx, Vy
void Chip8::op_8xy3(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        v_registers[x] ^= v_registers[y];
}

// 8xy4 - ADD Vx, Vy
void Chip8::op_8xy4(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        uint16_t sum = v_registers[x] + v_registers[y];
        if (sum > 255u) {
                v_registers[0xf] = 1;
        }
        else {
                v_registers[0xf] = 0;
        }
        v_registers[x] = sum & 0xffu;
}

// 8xy5 - SUB Vx, Vy
void Chip8::op_8xy5(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        if (v_registers[x] > v_registers[y]) {
		v_registers[0xf] = 1;
	}
	else {
		v_registers[0xf] = 0;
	}

	v_registers[x] -= v_registers[y];
}

// 8xy6 - SHR Vx {, Vy}
void Chip8::op_8xy6(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        v_registers[0xF] = (v_registers[x] & 0x1u);
	v_registers[x] >>= 1;
}

// 8xy7 - SUBN Vx, Vy
void Chip8::op_8xy7(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        if (v_registers[y] > v_registers[x]) {
		v_registers[0xF] = 1;
	}
	else {
		v_registers[0xF] = 0;
	}
	v_registers[x] = v_registers[y] - v_registers[x];
}

// 8xyE - SHL Vx {, Vy}
void Chip8::op_8xye(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        v_registers[0xF] = (v_registers[x] & 0x80u) >> 7u;
	v_registers[x] <<= 1;
}

// 9xy0 - SNE Vx, Vy
void Chip8::op_9xy0(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        if (v_registers[x] != v_registers[y]) {
                pc += 2;
        }
}

// Annn - LD I, addr
void Chip8::op_annn(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        index = nnn;
}

// Bnnn - JP V0, addr
void Chip8::op_bnnn(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        pc = v_registers[0] + nnn;
}

// Cxkk - RND Vx, byte
void Chip8::op_cxkk(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{

}

// Dxyn - DRW Vx, Vy, nibble
void Chip8::op_dxyn(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        uint8_t x_c = v_registers[x] % 64;
        uint8_t y_c = v_registers[y] % 32;
        v_registers[0xF] = 0u;
        // std::cout << "drawing to " << +x_c << "," << +y_c << "\n";
        for (long i = 0; i < n; ++i) {
                uint8_t spr_byte = index + i;
                for (long j = 0; j < 8; ++j) {
                        uint8_t spr_pixel = spr_byte & (0x80u >> j);
                        uint32_t scr_pixel = display[((y_c + i) * 64) + (x_c + j)];

			if (spr_pixel) {
				if (scr_pixel == 0xFFFFFFFF) {
					v_registers[0xF] = 1;
				}
				display[((y_c + i) * 64) + (x_c + j)] ^= 1;
			}

                }
        }
}

// Ex9E - SKP Vx
void Chip8::op_ex9e(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{

}

// ExA1 - SKNP Vx
void Chip8::op_exa1(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{

}

// Fx07 - LD Vx, DT
void Chip8::op_fx07(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{

}

// Fx0A - LD Vx, K
void Chip8::op_fx0a(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{

}

// Fx15 - LD DT, Vx
void Chip8::op_fx15(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{

}

// Fx18 - LD ST, Vx
void Chip8::op_fx18(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{

}

// Fx1E - ADD I, Vx
void Chip8::op_fx1e(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{

}

// Fx29 - LD F, Vx
void Chip8::op_fx29(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{

}

// Fx33 - LD B, Vx
void Chip8::op_fx33(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{

}

// Fx55 - LD [I], Vx
void Chip8::op_fx55(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{

}

// Fx65 - LD Vx, [I]
void Chip8::op_fx65(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{

}