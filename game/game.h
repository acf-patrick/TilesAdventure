#pragma once

#include <cstdlib>
#include <memory>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "core/const.h"
#include "core/ECS/entity.h"
#include "texture/texture.h"
#include "physics/physics.h"
#include "core/scene/scene.h"
#include "renderer/renderer.h"
#include "renderer/renderer.h"
#include "core/ECS/component.h"
#include "core/script/script.h"

// Main game class
class Game 
{
private:
    // The main window
    SDL_Window* window_;

    // The main renderer
    Renderer renderer_;

    // Texture manager
    TextureManager texture_;

    // Event handler
    EventHandler eventHandler_;

    // The active scene
    Scene activeScene_;

    // The Physic System
    PhysicSystem physicSystem_;

    // last tick recorded
    Uint32 lastTick_;

    // frames per second
    int fps_ = 60;

    bool update();

    void render();
    
public:
    Game(const std::string& config);

    ~Game();

    void run();
};

