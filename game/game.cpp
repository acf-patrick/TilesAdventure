#include "game.h"
#include "event/event.h"

#include <utility>
#include <iostream>
#include <yaml-cpp/yaml.h>
#include <tileson/tileson.hpp>

Game* Game::instance = nullptr;

Game::Game(const std::string& config)
{
	YAML::Node node = YAML::LoadFile(config);
	if (!node)
	{
		std::cerr << "Configuration file not found!" << std::endl;
		exit(1);
	}

    // Init
    SDL_Init( SDL_INIT_VIDEO );
    
    // Creating window ans renderer
    SDL_CreateWindowAndRenderer( node["width"].as<int>(), node["height"].as<int>(), SDL_WINDOW_SHOWN, &window_, &renderer_.renderer_ );
    
    if( !window_ || !renderer_.renderer_ )
        throw std::logic_error( SDL_GetError() );
        
    SDL_SetWindowPosition( window_, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED );
    SDL_SetWindowTitle( window_, node["name"].as<std::string>().c_str() );
        
    // Setting the active scene
    activeScene_ = Scene::Get();
	activeScene_->load(node["mainScene"].as<std::string>());

	eventHandler_ = new EventHandler;
}

Game::~Game()
{
    // Destroying window and renderer
    SDL_DestroyWindow( window_ );
    SDL_Quit();
}

Game* Game::Get()
{
    return instance;
}

void Game::run()
{
    // Main game loop
    GameState state = GameState::RUNNING;
    
    while( state == GameState::RUNNING )
    {
        manageEvent();
        update();
        render();
    }
}

void Game::manageEvent()
{
    // Polling possibles events
    SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		// If quitting game
		if( event.type == SDL_QUIT )
			exit( EXIT_SUCCESS );

		if (event.type == SDL_KEYDOWN)
		{
			SDL_Keycode index = event.key.keysym.sym;
			eventHandler_->keys[index] = true;
		}
		 
		if (event.type == SDL_KEYUP)
		{
			SDL_Keycode index = event.key.keysym.sym;
			eventHandler_->keys[index] = false;
		}
	}
}

void Game::update()
{
    auto curr = SDL_GetTicks();
    activeScene_->update( curr - last );
    last = curr;
}

void Game::render()
{
    activeScene_->render(renderer_.renderer_);
	renderer_.update();
}
