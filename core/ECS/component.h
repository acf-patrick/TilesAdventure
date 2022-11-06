#pragma once

#include <memory>

#include "core/const.h"
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
		Sprite(const std::string& textureTag)
		{
			texture = TextureManager::Get()->get(textureTag);

			if (!texture)
			{
				std::string error(textureTag);
				error += " : texture not found!";
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
		SDL_Point spacing = { 0, 0 };

		// Blank space around the image (in pixels)
		SDL_Point margin = { 0, 0 };

		// Index of the tile to be used
		int index = 0;
	};

	class Tilemap 
	{
		static tson::Tileson parser;
		std::unique_ptr<tson::Map> map_;

	public:
		Tilemap(const std::string& map)
		{
			map_ = parser.parse(fs::path(MAPS_PATH + map + ".json"));
			if (map_->getStatus() != tson::ParseStatus::OK)
				throw std::logic_error("Tilemap error : " + map_->getStatusMessage());
		}

		friend class TilemapRenderer;
	};

}