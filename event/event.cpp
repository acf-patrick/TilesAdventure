#include "event.h"

EventHandler* EventHandler::instance;

EventHandler* EventHandler::Get()
{
	if (!instance)
		new EventHandler;
	return instance;
}

bool EventHandler::getKey(SDL_Keycode index)
{
	return keys[index];
}

SDL_Point EventHandler::getMousePosition()
{
	return mousePosition;
}

EventHandler::EventHandler()
{
	instance = this;
}
