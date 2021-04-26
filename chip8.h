#pragma once

#include <array>
#include <stdint.h>
#include <string>
#include <random>


const unsigned int KEY_COUNT = 16;
const unsigned int MEMORY_SIZE = 4096;
const unsigned int REGISTER_COUNT = 16;
const unsigned int STACK_LEVELS = 16;
const unsigned int VIDEO_HEIGHT = 32;
const unsigned int VIDEO_WIDTH = 64;

class Chip8
{


public:
        Chip8();

        void load_rom(std::string filename);
        void dump_mem();
        void dump_display();
        void dump_regs();
        void cycle();

        std::array<uint32_t, 2048> display{};
        std::array<uint8_t, KEY_COUNT> keypad{};


private:

        //Instructions

        // 0000 - NULL
        void op_null(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn); 

        // 00E0 - CLS
        void op_00e0(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 00EE - RET
        void op_00ee(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);
        
        // 0nnn - SYS addr
        void op_0nnn(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 1nnn - JP addr
        void op_1nnn(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 2nnn - CALL addr
        void op_2nnn(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 3xkk - SE Vx, byte
        void op_3xkk(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 4xkk - SNE Vx, byte
        void op_4xkk(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 5xy0 - SE Vx, Vy
        void op_5xy0(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 6xkk - LD Vx, byte
        void op_6xkk(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 7xkk - ADD Vx, byte
        void op_7xkk(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 8xy0 - LD Vx, Vy
        void op_8xy0(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 8xy1 - OR Vx, Vy
        void op_8xy1(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);
        
        // 8xy2 - AND Vx, Vy
        void op_8xy2(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 8xy3 - XOR Vx, Vy
        void op_8xy3(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 8xy4 - ADD Vx, Vy
        void op_8xy4(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 8xy5 - SUB Vx, Vy
        void op_8xy5(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 8xy6 - SHR Vx {, Vy}
        void op_8xy6(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 8xy7 - SUBN Vx, Vy
        void op_8xy7(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 8xyE - SHL Vx {, Vy}
        void op_8xye(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // 9xy0 - SNE Vx, Vy
        void op_9xy0(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // Annn - LD I, addr
        void op_annn(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // Bnnn - JP V0, addr
        void op_bnnn(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // Cxkk - RND Vx, byte
        void op_cxkk(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);
        
        // Dxyn - DRW Vx, Vy, nibble
        void op_dxyn(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // Ex9E - SKP Vx
        void op_ex9e(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // ExA1 - SKNP Vx
        void op_exa1(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // Fx07 - LD Vx, DT
        void op_fx07(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // Fx0A - LD Vx, K
        void op_fx0a(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // Fx15 - LD DT, Vx
        void op_fx15(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // Fx18 - LD ST, Vx
        void op_fx18(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // Fx1E - ADD I, Vx
        void op_fx1e(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // Fx29 - LD F, Vx
        void op_fx29(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // Fx33 - LD B, Vx
        void op_fx33(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // Fx55 - LD [I], Vx
        void op_fx55(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        // Fx65 - LD Vx, [I]
        void op_fx65(uint8_t x, uint8_t y, uint8_t n, uint16_t kk, uint16_t nnn);

        std::array<uint8_t, 16> v_registers{};
        std::array<uint8_t, 4096> memory{};
        std::array<uint16_t, 16> stack{};
        uint16_t index;
        uint8_t sound_timer;
        uint8_t delay_timer;
        uint16_t pc;
        uint8_t sp;
        std::array<uint8_t, 80> fontset{};

        std::default_random_engine rand_gen;
	std::uniform_int_distribution<uint8_t> rand_byte;



};