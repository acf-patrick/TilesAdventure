#pragma once

#include <map>
#include <SDL2/SDL.h>

class Game;

class EventHandler
{
private:
	std::map<SDL_Keycode, bool> keys;

	SDL_Point mousePosition;

	static EventHandler* instance;

	EventHandler();

public:
	static EventHandler* Get();

	bool getKey(SDL_Keycode index);

	SDL_Point getMousePosition();

	friend class Game;
};
