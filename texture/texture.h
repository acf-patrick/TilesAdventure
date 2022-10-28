#pragma once

#include <map>
#include <iostream>
#include <SDL2/SDL.h>
#include "renderer/renderer.h"

class Renderer;

// Manage texture Load/Unload/Retreiving
class TextureManager
{
public:
	static TextureManager* Get();

	~TextureManager();
	
	// Retrieve texture by tag
	SDL_Texture* get(const std::string& tag);

	// Load texture and associate it with a tag
	SDL_Texture* load(const std::string& filePath, const std::string& tag = "");

	SDL_Texture* load(const std::string& filePath, const SDL_Color& colorkey, const std::string& tag = "");

	// Unload texture by tag
	void erase(const std::string& tag);

private:
	static TextureManager* instance;

	TextureManager();

	std::map<std::string, SDL_Texture*> textures_;
};
