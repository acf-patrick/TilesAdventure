#pragma once

#include <cstdlib>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "core/ECS/entity.h"
#include "texture/texture.h"
#include "renderer/renderer.h"
#include "core/const.h"
#include "renderer/renderer.h"
#include "core/ECS/component.h"
#include "core/script/script.h"
#include "core/scene/scene.h"

// enumeration for the game state
enum class GameState
{
    RUNNING,
    STOPPED,
    HALTED
};

// Main game class
class Game {

    private:
        // The main window
        SDL_Window* window_;

        // The main renderer
        Renderer renderer_;

        // The instance of Game
        static Game* instance;

        // The active scene
        Scene* activeScene_;

		// Event handler
		EventHandler* eventHandler_;

    public:
        Game(const std::string& config);

        ~Game();

        static Game* Get();

        // Main loop
        void run();

        void manageEvent();

        void update();

        void render();

        Uint32 last;

};

