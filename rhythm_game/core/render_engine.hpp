#pragma once

#include <SDL3/SDL.h>
#include <memory>
#include <functional>

namespace core
{
    struct ObjectPosition
    {
        float x;
        float y;
    };

    struct ObjectColor
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    enum RenderableScene
    {
        NONE,
        EXAMPLE,
        ALTERNATIVE,
    };

    enum RenderableObject
    {
        Tile
    };

    class RenderEngine
    {
        public:
            virtual void setup() = 0;
            virtual void render() = 0;
            virtual void sleep() = 0;
            virtual bool isActive() = 0;
            virtual void stop() = 0;
            virtual void clear() = 0;

            virtual void setScene(RenderableScene) = 0;
            virtual void spawnObject(RenderableObject) = 0;
    };

    class RenderEngineSDL3 : public RenderEngine
    {
        public:
            ~RenderEngineSDL3();
            void setup() override;
            void render() override;
            void sleep() override;
            bool isActive() override;
            void stop() override;
            void clear() override;

            void setScene(RenderableScene) override;
            void spawnObject(RenderableObject) override;
        private:
            SDL_Window* window = nullptr;
            SDL_Renderer* renderer = nullptr;

            bool active = true;

            RenderableScene currentScene = RenderableScene::NONE;
            static std::function<void(void)> sceneCallback;

            void renderExampleScene();
            void renderAlternativeScene();
    };

    std::unique_ptr<RenderEngine> determineRenderStrategy();
}
