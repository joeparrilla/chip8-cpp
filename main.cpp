#include <iostream>
#include "platform.h"
#include <chrono>
#include </home/joe/Projects/Personal/chip8-cpp/chip8.h>

int main(int argc, char ** argv)
{
	if (argc != 4) {
		std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
		std::exit(EXIT_FAILURE);
	}

	int video_scale = std::stoi(argv[1]);
	int cycle_delay = std::stoi(argv[2]);
	char const* rom_file_name = argv[3];

	Chip8 chip8;
	chip8.load_rom(rom_file_name);

	Platform platform("CHIP-8 Emulator", VIDEO_WIDTH * video_scale, VIDEO_HEIGHT * video_scale, VIDEO_WIDTH, VIDEO_HEIGHT);
	int video_pitch = sizeof(chip8.display[0]) * VIDEO_WIDTH;

	auto lastCycleTime = std::chrono::high_resolution_clock::now();
	bool quit = false;

	while (!quit)
	{
		quit = platform.ProcessInput(chip8.keypad.data());

		auto currentTime = std::chrono::high_resolution_clock::now();
		float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

		if (dt > cycle_delay) {
			lastCycleTime = currentTime;

			chip8.cycle();
			// chip8.dump_regs();
			// platform.Update(chip8.video, videoPitch);
			// chip8.dump_display();
			platform.Update(chip8.display.data(), video_pitch);
		}
	}

	return 0;
}
