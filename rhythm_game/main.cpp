#include "stdint.h"
#include <iostream>
#include <fstream>
#include <string>
#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_thread.h"
#include <nlohmann/json.hpp>

int main(int argc, char* argv[]) {
    std::cout << "Running..." << std::endl;

    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input_wav_file>" << std::endl;
        return 1;
    }

    std::string jsonBeatMap = "C:\\Users\\jayso\\Documents\\Github\\luna\\build\\map_generator\\beat_map.json";

    std::ifstream file(jsonBeatMap);
        if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << jsonBeatMap << std::endl;
        return 1;
    }

    std::vector<double> beatList{};

    try {
        nlohmann::json data_array;
        file >> data_array;
        
        for (const auto& item : data_array)
        {
            if (item.contains("beat_trigger")) {
                double trigger_time = item["beat_trigger"].get<double>();
                beatList.push_back(trigger_time);
            }
        }
    } catch (const nlohmann::json::parse_error& e) {
        std::cerr << "JSON Parse Error" << e.what() << std::endl;
    }

    const char* filename = argv[1];

    SoLoud::Soloud gSoloud;
    SoLoud::Wav gWave;

    gSoloud.init();

    gWave.load(filename);

    SoLoud::handle wavSound = gSoloud.play(gWave);

    double beatIndex = 0;
    double nextBeatTrigger = beatList[0];
    int maxBeatIndex = beatList.size();

    double time_off_correction = 0.0431383125;

	while (gSoloud.getActiveVoiceCount() > 0)
	{
        double currentTime = gSoloud.getStreamTime(wavSound);
        double correctedTime = currentTime - time_off_correction;

        if (correctedTime >= nextBeatTrigger && beatIndex <= maxBeatIndex)
        {
            std::cout << "Music time: " << correctedTime << ", [" << beatIndex << "/" << maxBeatIndex << "] triggered at: " << nextBeatTrigger << std::endl;
            nextBeatTrigger = beatList[beatIndex++];
        }
    
		SoLoud::Thread::sleep(10);
	}

    gSoloud.deinit();
    
    return 0;
}