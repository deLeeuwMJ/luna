#include "stdint.h"
#include <iostream>

#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_thread.h"

int main(int argc, char* argv[]) {
    std::cout << "Running..." << std::endl;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_wav_file>" << std::endl;
        return 1;
    }

    const char* filename = argv[1];

    SoLoud::Soloud gSoloud;
    SoLoud::Wav gWave;

    gSoloud.init();

    gWave.load(filename);

    gSoloud.play(gWave);

	while (gSoloud.getActiveVoiceCount() > 0)
	{
		SoLoud::Thread::sleep(100);
	}

    gSoloud.deinit();
    
    return 0;
}