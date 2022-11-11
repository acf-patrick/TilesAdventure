#include "event.h"

#include <iostream>
#include <SDL2/SDL.h>

EventHandler* EventHandler::instance;

EventHandler* EventHandler::Get()
{
	if (!instance)
		new EventHandler;
	return instance;
}

EventHandler::EventHandler()
{
	instance = this;
}

EventHandler::~EventHandler()
{
	/* for (auto joystick : joysticks_)
		SDL_JoystickClose(joystick); */
}

void EventHandler::init()
{
	auto joystickCount = SDL_NumJoysticks();
	for (int i = 0; i < joystickCount; ++i)
		joysticks_.emplace_back(SDL_JoystickOpen(i));
}

bool EventHandler::getKey(SDL_Keycode index)
{
	return keys_[index];
}

bool EventHandler::getMouseButton(int buttons)
{
	return buttons & SDL_GetMouseState(NULL, NULL);
}

SDL_Point EventHandler::getMousePosition()
{
	SDL_Point pos;
	SDL_GetMouseState(&pos.x, &pos.y);
	return pos;
}

bool EventHandler::getJoystickButton(int buttonID, int joystickID)
{
	if (joysticks_.empty())
	{
		init();
		if (joysticks_.empty())
			return false;
	}

	if (joystickID >= joysticks_.size())
		return false;

	auto joystick = joysticks_[joystickID];
	SDL_JoystickUpdate();
	return SDL_JoystickGetButton(joystick, buttonID);
}

int EventHandler::getJoystickAxis(int axisID, int joystickID)
{
	if (joysticks_.empty())
	{
		init();
		if (joysticks_.empty())
			return false;
	}

	if (joystickID >= joysticks_.size())
		return false;
	
	auto joystick = joysticks_[joystickID];
	SDL_JoystickUpdate();
	return SDL_JoystickGetAxis(joystick, axisID);
}

bool EventHandler::update()
{
    // Polling possibles events
    SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		// If quitting game
		if( event.type == SDL_QUIT )
			return false;

		if (event.type == SDL_KEYDOWN)
		{
			SDL_Keycode index = event.key.keysym.sym;
			keys_[index] = true;

			if (index == SDLK_ESCAPE)
				return false;
		}

		if (event.type == SDL_KEYUP)
		{
			SDL_Keycode index = event.key.keysym.sym;
			keys_[index] = false;
		}
	}

	return true;
}
