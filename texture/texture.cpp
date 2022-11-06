#include "texture.h"
#include "core/const.h"
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

std::string TextureManager::createTag(std::string path)
{
	auto end = path.find_last_of('(');
	if (end == path.npos)
		end = path.find_last_of('.');
	else
		end--;
	if (end == path.npos)
		return path;
	path = path.substr(0, end);
	for (auto& car : path)
		if (car == ' ' or car == '/')
			car = '_';
	return path;
}

SDL_Texture* TextureManager::get(const std::string& tag)
{
	if (textures_.find(tag) == textures_.end())
	{
		std::cerr << tag << " : texture not found" << std::endl;
		return nullptr;
	}
	return textures_[tag];
}

SDL_Texture* TextureManager::load(const std::string& filePath, const std::string& t)
{
	std::string tag(t);
	if (tag.empty())
		tag = createTag(filePath);
	
	if (textures_.find(tag) != textures_.end())
		return textures_[tag];

	textures_[tag] = IMG_LoadTexture(Renderer::Get()->renderer_, (IMAGES_PATH + filePath).c_str());
	return textures_[tag];
}

SDL_Texture* TextureManager::load(const std::string& filePath, const SDL_Color& colorkey, const std::string& t)
{
	std::string tag(t);
	if (tag.empty())
		tag = createTag(filePath);

	if (textures_.find(tag) != textures_.end())
		return textures_[tag];

	auto surface = IMG_Load((IMAGES_PATH + filePath).c_str());
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, colorkey.r, colorkey.g, colorkey.b));
	textures_[tag] = SDL_CreateTextureFromSurface(Renderer::Get()->renderer_, surface);
	
	SDL_FreeSurface(surface);

	return textures_[tag];
}

void TextureManager::erase(const std::string & tag)
{
	if (textures_.find(tag) == textures_.end())
		throw std::out_of_range("Texture tag not found.");

	SDL_DestroyTexture(textures_.at(tag));
	
	textures_.erase(tag);
}
