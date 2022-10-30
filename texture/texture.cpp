#include "texture.h"
#include "renderer/renderer.h"

#include <SDL2/SDL_image.h>

TextureManager* TextureManager::instance = nullptr;

TextureManager* TextureManager::Get()
{
	if (!instance)
		new TextureManager;
	return instance;
}

TextureManager::TextureManager()
{
	instance = this;
}

TextureManager::~TextureManager()
{
	for (auto[tag, texture] : textures_)
		SDL_DestroyTexture(texture);
	textures_.clear();

	instance = nullptr;
}

SDL_Texture* TextureManager::get(const std::string& tag)
{
	if (textures_.find(tag) == textures_.end())
		return nullptr;
	return textures_[tag];
}

SDL_Texture* TextureManager::load(const std::string& filePath, const std::string& t)
{
	std::string tag(t);
	if (tag.empty())
	{
		tag = filePath;
		auto slash = tag.find_last_of('/');
		if (slash == tag.npos)
			slash = tag.find_last_of('\\');
		auto dot = tag.find_last_of('.');
		tag = tag.substr(slash + 1, dot - slash - 1);
	}
	
	if (textures_.find(tag) != textures_.end())
		return textures_[tag];

	textures_[tag] = IMG_LoadTexture(Renderer::Get()->renderer_, filePath.c_str());
	return textures_[tag];
}


SDL_Texture* TextureManager::load(const std::string& filePath, const SDL_Color& colorkey, const std::string& t)
{
	std::string tag(t);
	if (tag.empty())
	{
		tag = filePath;
		auto slash = tag.find_last_of('/');
		if (slash == tag.npos)
			slash = tag.find_last_of('\\');
		auto dot = tag.find_last_of('.');
		tag = tag.substr(slash + 1, dot - slash - 1);
	}

	if (textures_.find(tag) != textures_.end())
		return textures_[tag];

	auto surface = IMG_Load(filePath.c_str());
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, colorkey.r, colorkey.g, colorkey.b));
	textures_[tag] = SDL_CreateTextureFromSurface(Renderer::Get()->renderer_, surface);
	
	SDL_FreeSurface(surface);

	return textures_[tag];
}

void TextureManager::erase(const std::string & tag)
{
	if (textures_.find(tag) == textures_.end())
		throw std::out_of_range("Tag of texture not found.");

	SDL_DestroyTexture(textures_.at(tag));
	
	textures_.erase(tag);
}
