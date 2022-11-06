#include "renderer.h"

#include <iostream>
#include <stdexcept>

Renderer* Renderer::instance = nullptr;

Renderer* Renderer::Get()
{
	if (!instance)
		throw std::logic_error("Renderer can't have more than one instance.");

	return instance;
}

void Renderer::init(SDL_Window* window)
{
	window_ = window;
	renderer_ = SDL_CreateRenderer(window, -1, SDL_RENDERER_TARGETTEXTURE);
	if (!renderer_)
		throw std::logic_error( SDL_GetError() );
}

Renderer::Renderer()
{
	instance = this;
}

Renderer::~Renderer()
{
	SDL_DestroyTexture(target_);
	SDL_DestroyRenderer(renderer_);
}

Vector Renderer::getOutputScale() const
{
	return { scale_.x, scale_.y };
}

void Renderer::setOutputScale(float x, float y)
{
	scale_.x = x;
	scale_.y = y;
	
	SDL_GetWindowSize(window_, &size_.x, &size_.y);
	size_.x *= x;
	size_.y *= y;

	target_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size_.x, size_.y);
	if (!target_)
		throw std::logic_error( SDL_GetError() );

	// recompute window size
	if (x != 1 or y != 1)
		SDL_SetWindowSize(window_, size_.x, size_.y);
}

void Renderer::prepare()
{
	SDL_SetRenderTarget(renderer_, target_);
	SDL_SetRenderDrawColor(renderer_, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	SDL_RenderClear(renderer_);
}

void Renderer::update()
{
	SDL_SetRenderTarget(renderer_, NULL);
	
	SDL_Rect dst = { 0, 0, scale_.x * size_.x, scale_.y * size_.y };
	SDL_RenderCopy(renderer_, target_, NULL, &dst);

	SDL_RenderPresent(renderer_);
}

SDL_Renderer* Renderer::get()
{
	return renderer_;
}