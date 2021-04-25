#include <iostream>
#include <chrono>
#include </home/joe/Projects/Personal/chip8-cpp/chip8.h>

int main(int, char**) {
    Chip8 chip8;
    chip8.load_rom("/home/joe/Projects/Personal/chip8-cpp/roms/IBM Logo.ch8");
    // chip8.dump_mem();

    auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;

	while (!quit)
	{
		// quit = platform.ProcessInput(chip8.keypad);

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		// if (dt > cycleDelay)
        if (dt > 10)
		{   
			lastCycleTime = currentTime;

			chip8.cycle();
            // chip8.dump_regs();
			// platform.Update(chip8.video, videoPitch);
            chip8.dump_display();
		}
	}

	return 0;
}
