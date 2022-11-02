#pragma once

#include <map>
#include <vector>
#include <SDL2/SDL.h>

class EventHandler
{
private:
	std::map<SDL_Keycode, bool> keys_;

	std::vector<SDL_Joystick*> joysticks_;

	int joystickButtonsCount_ = 0;

	static EventHandler* instance;

private:
	void init();

	// Got EXIT request if return false
	bool update();

	EventHandler();

public:
	static EventHandler* Get();

	~EventHandler();

	bool getKey(SDL_Keycode index);

	bool getJoystickButton(int joystickID, int buttonID);

	SDL_Point getMousePosition();

	bool getMouseButton(int buttons);

	friend class Game;
};
