#include "game.h"
#include "event/event.h"

#include <map>
#include <utility>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <tileson/tileson.hpp>

Game::Game(const std::string& config)
{
	YAML::Node node = YAML::LoadFile(config);
	if (!node)
	{
		std::cerr << "Configuration file not found!" << std::endl;
		exit(1);
	}

    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK );

    // Creating window ans renderer
    SDL_CreateWindowAndRenderer( node["width"].as<int>(), node["height"].as<int>(), SDL_WINDOW_SHOWN, &window_, &renderer_.renderer_ );

    if( !window_ || !renderer_.renderer_ )
        throw std::logic_error( SDL_GetError() );

    SDL_SetWindowPosition( window_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED );
    SDL_SetWindowTitle( window_, node["name"].as<std::string>().c_str() );

    /* Loading ressources */

    auto rsc = node["resources"];

    auto images = rsc["images"].as<std::map<std::string, std::string>>();

    for (auto& [tag, path]: images)
        texture_.load(path, tag);

    // Setting the active scene
	activeScene_.load(node["main"].as<std::string>());
}

Game::~Game()
{
    SDL_DestroyWindow( window_ );
    SDL_Quit();
}

void Game::run()
{
    // Main game loop
    GameState state = GameState::RUNNING;

    while( state == GameState::RUNNING )
    {
        if (!eventHandler_.update())
            break;
        update();
        render();
    }
}

void Game::update()
{
    auto curr = SDL_GetTicks();
    activeScene_.update( curr - last );
    last = curr;
}

void Game::render()
{
    renderer_.clear();
    activeScene_.render(renderer_.renderer_);
	renderer_.update();
}
