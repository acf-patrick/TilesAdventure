#pragma once

#include "texture/texture.h"
#include "tileson/tileson.hpp"
#include "core/geometry/geometry.h"

namespace ECS {
	struct Transform
	{
		Point position = { 0.0f, 0.0f };
		Vector scale = { 1.0f, 1.0f };
		float rotation = 0.0f;
		size_t zIndex = 0;
	};

	struct Sprite
	{
		Sprite(const std::string& path)
		{
			texture = TextureManager::Get()->load(path);

			if (!texture)
			{
				std::string error(path);
				error += " not found!";
				throw std::logic_error(error.c_str());
			}

			source.x = source.y = 0;
			SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);
		}

		Sprite(const std::string& path, const SDL_Color& colorkey)
		{
			texture = TextureManager::Get()->load(path, colorkey);

			if (!texture)
			{
				std::string error(path);
				error += " not found!";
				throw std::logic_error(error.c_str());
			}

			source.x = source.y = 0;
			SDL_QueryTexture(texture, NULL, NULL, &source.w, &source.h);
		}

		// Use whole image if false
		bool sliced = false;

		// Image to be rendered
		SDL_Texture* texture;

		// Part of the image to be rendered
		SDL_Rect source;

		// Size of each tile of the image
		SDL_Point slice;

		// Space between tiles (in pixels)
		SDL_Point spacing;

		// Blank space around the image (in pixels)
		SDL_Point margin;

		// Index of the tile to be used
		int index = 0;
	};
}