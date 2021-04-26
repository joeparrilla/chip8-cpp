#include "chip8.h"

#include <fstream>
#include <iostream>
#include <bitset>
#include <chrono>

const unsigned int FONTSET_SIZE = 80;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int START_ADDRESS = 0x200;

Chip8::Chip8()
    : pc(0x200u), index(0x0u), sound_timer(0x0u), delay_timer(0x0u), sp(0x0u),
      rand_gen(std::chrono::system_clock::now().time_since_epoch().count())
{

        rand_byte = std::uniform_int_distribution<uint8_t>(0, 255u);

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

        for (unsigned int i = 0; i < FONTSET_SIZE; ++i)
        {
                memory[FONTSET_START_ADDRESS + i] = fontset[i];
        }
}

void Chip8::load_rom(std::string filename)
{
        std::ifstream file(filename, std::ios::binary | std::ios::ate);

        if (file.is_open())
        {
                std::streampos size = file.tellg();
                char *buffer = new char[size];
                file.seekg(0, std::ios::beg);
                file.read(buffer, size);
                file.close();

                for (long i = 0; i < size; ++i)
                {
                        memory[START_ADDRESS + i] = buffer[i];
                }
                delete[] buffer;
        }
}

void Chip8::dump_mem()
{
        for (int i{0}; auto &item : memory)
        {
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
        for (int i{0}; auto &item : v_registers)
        {
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
        for (int i{0}; auto &pixel : display)
        {
                std::bitset<8> converted(pixel);
                std::cout << converted.to_ulong();
                if ((i + 1) % 64 == 0)
                {
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
        if (op == 0x0)
        {
                addtl_op = opcode & 0xfu;
                if (addtl_op == 0x0)
                {
                        op_00e0(x, y, n, kk, nnn);
                }
                else
                {
                        op_00ee(x, y, n, kk, nnn);
                }
        }
        else if (op == 0x1)
        {
                op_1nnn(x, y, n, kk, nnn);
        }
        else if (op == 0x2)
        {
                op_2nnn(x, y, n, kk, nnn);
        }
        else if (op == 0x3)
        {
                op_3xkk(x, y, n, kk, nnn);
        }
        else if (op == 0x4)
        {
                op_4xkk(x, y, n, kk, nnn);
        }
        else if (op == 0x5)
        {
                op_5xy0(x, y, n, kk, nnn);
        }
        else if (op == 0x6)
        {
                op_6xkk(x, y, n, kk, nnn);
        }
        else if (op == 0x7)
        {
                op_7xkk(x, y, n, kk, nnn);
        }
        else if (op == 0x8)
        {
                addtl_op = opcode & 0xfu;
                if (addtl_op == 0x0)
                {
                        op_8xy0(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x1)
                {
                        op_8xy1(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x2)
                {
                        op_8xy2(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x3)
                {
                        op_8xy3(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x4)
                {
                        op_8xy4(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x5)
                {
                        op_8xy5(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x6)
                {
                        op_8xy6(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x7)
                {
                        op_8xy7(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0xe)
                {
                        op_8xye(x, y, n, kk, nnn);
                }
        }
        else if (op == 0x9)
        {
                op_9xy0(x, y, n, kk, nnn);
        }
        else if (op == 0xa)
        {
                op_annn(x, y, n, kk, nnn);
        }
        else if (op == 0xb)
        {
                op_bnnn(x, y, n, kk, nnn);
        }
        else if (op == 0xc)
        {
                op_cxkk(x, y, n, kk, nnn);
        }
        else if (op == 0xd)
        {
                op_dxyn(x, y, n, kk, nnn);
        }
        else if (op == 0xe)
        {
                addtl_op = opcode & 0xfu;
                if (addtl_op == 0xe)
                {
                        op_ex9e(x, y, n, kk, nnn);
                }
                else
                {
                        op_exa1(x, y, n, kk, nnn);
                }
        }
        else if (op == 0xf)
        {
                addtl_op = opcode & 0xffu;
                if (addtl_op == 0x07)
                {
                        op_fx07(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x0a)
                {
                        op_fx0a(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x15)
                {
                        op_fx15(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x18)
                {
                        op_fx18(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x1e)
                {
                        op_fx1e(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x29)
                {
                        op_fx29(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x33)
                {
                        op_fx33(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x55)
                {
                        op_fx55(x, y, n, kk, nnn);
                }
                else if (addtl_op == 0x65)
                {
                        op_fx65(x, y, n, kk, nnn);
                }
        }

        if (delay_timer > 0)
        {
                --delay_timer;
        }

        if (sound_timer > 0)
        {
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
        for (auto &value : display)
        {
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
void Chip8::op_1nnn(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
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
        if (v_registers[x] == kk)
                pc += 2;
}

// 4xkk - SNE Vx, byte
void Chip8::op_4xkk(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        if (v_registers[x] != kk)
                pc += 2;
}

// 5xy0 - SE Vx, Vy
void Chip8::op_5xy0(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        if (v_registers[x] == v_registers[y])
                pc += 2;
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
        if (sum > 255u)
        {
                v_registers[0xf] = 1;
        }
        else
        {
                v_registers[0xf] = 0;
        }
        v_registers[x] = sum & 0xffu;
}

// 8xy5 - SUB Vx, Vy
void Chip8::op_8xy5(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        if (v_registers[x] > v_registers[y])
        {
                v_registers[0xf] = 1;
        }
        else
        {
                v_registers[0xf] = 0;
        }

        v_registers[x] -= v_registers[y];
}

// 8xy6 - SHR Vx {, Vy}
void Chip8::op_8xy6(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        //v_registers[x] = v_registers[y];
        v_registers[0xF] = (v_registers[x] & 0x1u);
        v_registers[x] >>= 1;
}

// 8xy7 - SUBN Vx, Vy
void Chip8::op_8xy7(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        if (v_registers[y] > v_registers[x])
        {
                v_registers[0xF] = 1;
        }
        else
        {
                v_registers[0xF] = 0;
        }
        v_registers[x] = v_registers[y] - v_registers[x];
}

// 8xyE - SHL Vx {, Vy}
void Chip8::op_8xye(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        //v_registers[x] = v_registers[y];
        v_registers[0xF] = (v_registers[x] & 0x80u) >> 7u;
        v_registers[x] <<= 1;
}

// 9xy0 - SNE Vx, Vy
void Chip8::op_9xy0(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        if (v_registers[x] != v_registers[y])
        {
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
        v_registers[x] = rand_byte(rand_gen) & kk;
}

// Dxyn - DRW Vx, Vy, nibble
void Chip8::op_dxyn(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        uint8_t x_c = v_registers[x] % 64;
        uint8_t y_c = v_registers[y] % 32;
        v_registers[0xF] = 0u;
        // std::cout << "drawing to " << +x_c << "," << +y_c << "\n";
        for (unsigned int row = 0; row < n; ++row)
        {
                uint8_t spr_byte = memory[index + row];
                for (unsigned int col = 0; col < 8; ++col)
                {
                        uint8_t spr_pixel = spr_byte & (0x80u >> col);
                        uint32_t scr_pixel = display[((y_c + row) * 64) + (x_c + col)];

                        if (spr_pixel)
                        {
                                if (scr_pixel == 0xFFFFFFFF)
                                {
                                        v_registers[0xF] = 1;
                                }
                                display[((y_c + row) * 64) + (x_c + col)] ^= 0xffffffff;
                        }
                }
        }
}

// Ex9E - SKP Vx
void Chip8::op_ex9e(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        if (keypad[v_registers[x]])
        {
                pc += 2;
        }
}

// ExA1 - SKNP Vx
void Chip8::op_exa1(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        if (!keypad[v_registers[x]])
        {
                pc += 2;
        }
}

// Fx07 - LD Vx, DT
void Chip8::op_fx07(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        v_registers[x] = delay_timer;
}

// Fx0A - LD Vx, K
void Chip8::op_fx0a(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        if (keypad[0])
        {
                v_registers[x] = 0;
        }
        else if (keypad[1])
        {
                v_registers[x] = 1;
        }
        else if (keypad[2])
        {
                v_registers[x] = 2;
        }
        else if (keypad[3])
        {
                v_registers[x] = 3;
        }
        else if (keypad[4])
        {
                v_registers[x] = 4;
        }
        else if (keypad[5])
        {
                v_registers[x] = 5;
        }
        else if (keypad[6])
        {
                v_registers[x] = 6;
        }
        else if (keypad[7])
        {
                v_registers[x] = 7;
        }
        else if (keypad[8])
        {
                v_registers[x] = 8;
        }
        else if (keypad[9])
        {
                v_registers[x] = 9;
        }
        else if (keypad[10])
        {
                v_registers[x] = 10;
        }
        else if (keypad[11])
        {
                v_registers[x] = 11;
        }
        else if (keypad[12])
        {
                v_registers[x] = 12;
        }
        else if (keypad[13])
        {
                v_registers[x] = 13;
        }
        else if (keypad[14])
        {
                v_registers[x] = 14;
        }
        else if (keypad[15])
        {
                v_registers[x] = 15;
        }
        else
        {
                pc -= 2;
        }
}

// Fx15 - LD DT, Vx
void Chip8::op_fx15(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        delay_timer = v_registers[x];
}

// Fx18 - LD ST, Vx
void Chip8::op_fx18(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        sound_timer = v_registers[x];
}

// Fx1E - ADD I, Vx
void Chip8::op_fx1e(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        index += v_registers[x];
}

// Fx29 - LD F, Vx
void Chip8::op_fx29(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        index = FONTSET_START_ADDRESS + (5 * v_registers[x]);
}

// Fx33 - LD B, Vx
void Chip8::op_fx33(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{ //CHECK FOR ACCURACY
        uint8_t value = v_registers[x];

        // Ones-place
        memory[index + 2] = value % 10;
        value /= 10;

        // Tens-place
        memory[index + 1] = value % 10;
        value /= 10;

        // Hundreds-place
        memory[index] = value % 10;
}

// Fx55 - LD [I], Vx
void Chip8::op_fx55(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        for (uint8_t i = 0; i <= x; ++i)
        {
                memory[index + i] = v_registers[i];
        }
}

// Fx65 - LD Vx, [I]
void Chip8::op_fx65(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn)
{
        for (uint8_t i = 0; i <= x; ++i)
        {
                v_registers[i] = memory[index + i];
        }
}