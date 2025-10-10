#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

#include "stdint.h"
#include "core/sound_engine.hpp"
#include "core/render_engine.hpp"
#include "core/constants.hpp"
#include "component/map_loader.hpp"

int main(int argc, char* argv[]) {
    auto renderEngine = core::determineRenderStrategy();
    auto soundEngine = core::determineSoundStrategy();

    soundEngine->setup();
    renderEngine->setup();
    renderEngine->setScene(core::RenderableScene::EXAMPLE);

    auto beatMap = std::make_unique<MapLoader>();
    beatMap->loadBeatMap(FILE_PATH_BEAT_MAP);

    soundEngine->loadSoundFile(FILE_PATH_MUSIC_FILE);

    auto beatList = beatMap->beats();
    double beatIndex = 0;
    double nextBeatTrigger = beatList[0];
    int maxBeatIndex = beatList.size();

    SDL_Event event;

    while (renderEngine->isActive()) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                renderEngine->stop();
            }

            if (event.key.key == SDLK_Q && !soundEngine->isActive()) {
                soundEngine->playSound();
            }
        }

        if (soundEngine->isActive()) {
            auto time = soundEngine->getSoundTimePosition();
            if (time >= nextBeatTrigger && beatIndex <= maxBeatIndex)
            {
                std::cout << "Music time: " << time << ", [" << beatIndex << "/" << maxBeatIndex << "] triggered at: " << nextBeatTrigger << std::endl;
                nextBeatTrigger = beatList[beatIndex++];
                renderEngine->spawnObject(core::RenderableObject::Tile);
            }
        }

        renderEngine->render();
        renderEngine->sleep();
    }

    return 0;
}