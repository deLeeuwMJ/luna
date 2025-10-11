#include <iostream>

#include "render_engine.hpp"
#include "constants.hpp"

namespace core
{
    std::unique_ptr<RenderEngine> determineRenderStrategy()
    {
        return std::make_unique<RenderEngineSDL3>();
    }

    ObjectPosition randomPosition()
    {
        ObjectPosition pos;
        pos.x = static_cast<float>(rand() % (WINDOW_WIDTH - 50));
        pos.y = static_cast<float>(rand() % (WINDOW_HEIGHT - 50));
        return pos;
    }

    ObjectColor randomColor()
    {
        ObjectColor color;
        color.r = rand() % 256;
        color.g = rand() % 256;
        color.b = rand() % 256;
        color.a = 255;
        return color;
    }

    void RenderEngineSDL3::setup()
    {
        if (!SDL_Init(SDL_INIT_VIDEO)) {
            std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
            return;
        }

        if (!SDL_CreateWindowAndRenderer(
            WINDOW_TITLE,
            WINDOW_WIDTH, WINDOW_HEIGHT, 
            0,
            &window, &renderer)) 
        {
            std::cerr << "Window/Renderer creation failed: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return;
        }
    }

    void RenderEngineSDL3::render()
    {
        if (sceneCallback)
            sceneCallback();
    }

    void RenderEngineSDL3::clear()
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }

    void RenderEngineSDL3::stop()
    {
        this->active = false;
    }

    bool RenderEngineSDL3::isActive()
    {
        return this->active;
    }

    void RenderEngineSDL3::sleep()
    {
        SDL_Delay(1000/60);
    }

    std::function<void(void)> RenderEngineSDL3::sceneCallback = nullptr;

    void RenderEngineSDL3::setScene(RenderableScene scene)
    {
        if (this->currentScene == scene)
            return;

        this->currentScene = scene;
        std::cout << "Switched to "<< scene << std::endl;

        switch(scene)
        {
            case EXAMPLE:
                this->sceneCallback = [this]() {
                     this->renderExampleScene(); 
                    };
                break;
            case ALTERNATIVE:
                this->sceneCallback = [this]() { 
                    this->renderAlternativeScene();
                 };
        }
    }

    void RenderEngineSDL3::renderExampleScene()
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    void RenderEngineSDL3::renderAlternativeScene()
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    }

    void RenderEngineSDL3::spawnObject(RenderableObject object)
    {
        std::cout << "Spawning object..." << std::endl;
        if (object == Tile) {
            auto color = randomColor();
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderClear(renderer);
            SDL_RenderPresent(renderer);
        }
    }

    RenderEngineSDL3::~RenderEngineSDL3()
    {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}