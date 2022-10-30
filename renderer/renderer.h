#pragma once

#include <queue>
#include <SDL2/SDL.h>
#include <functional>

class Renderer
{
public:
	static Renderer* Get();

	~Renderer();

	SDL_Renderer* get();

	// Clear screen
	void clear();

	// Update screen
	void update();

	SDL_Color clearColor = { 0, 0, 0, 0xff };

private:
	static Renderer* instance;

	Renderer();

	SDL_Renderer* renderer_;

	friend class Game;
	friend class TextureManager;
};