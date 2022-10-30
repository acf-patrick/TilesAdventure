#include "renderer.h"

#include <iostream>
#include <stdexcept>

Renderer* Renderer::instance = nullptr;

Renderer* Renderer::Get()
{
	if (!instance)
		throw std::logic_error("You've made an error.");

	return instance;
}

Renderer::Renderer()
{
	instance = this;
}

Renderer::~Renderer()
{
	SDL_DestroyRenderer(renderer_);
}

void Renderer::update()
{
	SDL_RenderPresent(renderer_);
}

SDL_Renderer* Renderer::get()
{
	return renderer_;
}

void Renderer::clear()
{
	SDL_SetRenderDrawColor(renderer_, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	SDL_RenderClear(renderer_);
}