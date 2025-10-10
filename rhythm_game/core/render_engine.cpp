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
        // No-op for now, only one object
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

    void RenderEngineSDL3::setScene(RenderableScene scene)
    {
        // No-op for now, only one scene
    }

    void RenderEngineSDL3::spawnObject(RenderableObject object)
    {
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