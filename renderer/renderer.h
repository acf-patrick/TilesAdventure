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

	// Update screen
	void update();

private:
	static Renderer* instance;

	Renderer();

	SDL_Renderer* renderer_;

	friend class Game;
	friend class TextureManager;
};