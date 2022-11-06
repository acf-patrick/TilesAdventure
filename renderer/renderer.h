#pragma once

#include <queue>
#include <SDL2/SDL.h>
#include <functional>

#include "core/geometry/geometry.h"

class Renderer
{
public:
	static Renderer* Get();

	~Renderer();

	SDL_Renderer* get();

	// Get output scale
	Vector getOutputScale() const;

	// Set output scale
	void setOutputScale(float x, float y);

	// Prepare rendering target
	void prepare();

	// Update screen
	void update();

	SDL_Color clearColor = { 0, 0, 0, 0xff };

private:
	static Renderer* instance;

	void init(SDL_Window*);

	Renderer();

	SDL_FPoint scale_ = { 1.0, 1.0 };
	SDL_Point size_;

	SDL_Window* window_;
	SDL_Renderer* renderer_;
	SDL_Texture* target_;

	friend class Game;
	friend class TextureManager;
};