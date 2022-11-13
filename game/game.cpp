#include "game.h"
#include "timer/timer.h"
#include "event/event.h"

#include <map>
#include <utility>
#include <iostream>
#include <yaml-cpp/yaml.h>

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
    window_ = SDL_CreateWindow(
        node["name"].as<std::string>().c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        node["width"].as<int>(),
        node["height"].as<int>(),
        SDL_WINDOW_SHOWN );

    if( !window_)
        throw std::logic_error( SDL_GetError() );

    renderer_.init(window_);

    auto scale = node["outputScale"];
    if (scale)
    {
        auto s = scale.as<std::vector<float>>();
        renderer_.setOutputScale(s[0], s[1]);
    }

    auto fps = node["fps"];
    if (fps)
        fps_ = fps.as<int>();

    /* Loading ressources */

    auto rsc = node["resources"];

    auto images = rsc["images"].as<std::map<std::string, std::string>>();

    for (auto& [tag, path]: images)
        texture_.load(path, tag);

    // Setting the active scene
	activeScene_.load(node["main"].as<std::string>());
	eventHandler_.init();
}

Game::~Game()
{
    SDL_DestroyWindow( window_ );
    SDL_Quit();
}

void Game::run()
{
    while( true )
    {
        if (!eventHandler_.update())
            break;
        
        if (!update())
            break;

        render();
    }
}

bool Game::update()
{
    auto curr = SDL_GetTicks();
    auto dt = curr - lastTick_;

    physicSystem_.update(dt);

    for (auto timer : Timer::instances)
        if (!timer->call())
            break;

    activeScene_.update( dt );
    if (!activeScene_.running_)
        return false;

    if (dt < 1000.0 / fps_)
        SDL_Delay(1000.0 / fps_ - dt);

    lastTick_ = curr;
    return true;
}

void Game::render()
{
    renderer_.prepare();
    activeScene_.render(renderer_.renderer_);
	renderer_.update();
}
