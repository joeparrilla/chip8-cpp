#include <iostream>
#include </home/joe/Projects/Personal/chip8-cpp/chip8.h>

int main(int, char**) {
    std::cout << "Hello, world!\n";
    Chip8 chip8;
    chip8.load_rom("/home/joe/Projects/Personal/chip8-cpp/roms/IBM Logo.ch8");
    chip8.dump_mem();
}
