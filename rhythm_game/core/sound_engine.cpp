#include <iostream>

#include "soloud_thread.h"
#include "sound_engine.hpp"
#include "constants.hpp"

namespace core
{
    std::unique_ptr<SoundEngine> determineSoundStrategy()
    {
        return std::make_unique<SoundEngineSOLOUD>();
    }

    void SoundEngineSOLOUD::setup()
    {
        engine.init();
    }

    void SoundEngineSOLOUD::loadSoundFile(std::string file_path)
    {
        auto path = file_path.c_str();
        std::cout << "Loading file: " << path << std::endl;
        soundFile.load(path);
    }

    void SoundEngineSOLOUD::playSound()
    {
        soundHandle = engine.play(soundFile);
    }

    bool SoundEngineSOLOUD::isActive()
    {
        return engine.getActiveVoiceCount() > 0;
    }

    double SoundEngineSOLOUD::getSoundTimePosition()
    {
        return engine.getStreamTime(soundHandle) - SOLOUD_TIME_CORRECTION;
    }

    void SoundEngineSOLOUD::sleep()
    {
        SoLoud::Thread::sleep(10);
    }

    SoundEngineSOLOUD::~SoundEngineSOLOUD()
    {
        engine.deinit();
    }
}