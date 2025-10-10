#pragma once

#include <string>
#include <memory>

#include "soloud.h"
#include "soloud_wav.h"

namespace core
{   
    class SoundEngine
    {
        public:
            virtual void setup() = 0;
            virtual void loadSoundFile(std::string) = 0;
            virtual void playSound() = 0;
            virtual bool isActive() = 0;
            virtual double getSoundTimePosition() = 0;
            virtual void sleep() = 0;
    };

    class SoundEngineSOLOUD : public SoundEngine
    {
        public:
            ~SoundEngineSOLOUD();
            void setup() override;
            void loadSoundFile(std::string) override;
            void playSound() override;
            bool isActive() override;
            double getSoundTimePosition() override;
            void sleep() override;
        private:
            SoLoud::Soloud engine;
            SoLoud::Wav soundFile;
            SoLoud::handle soundHandle;
    };

    std::unique_ptr<SoundEngine> determineSoundStrategy();
}
