#pragma once

#include <memory>

#include "core/const.h"
#include "texture/texture.h"
#include "physics/physics.h"
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

	class RigidBody
	{
		enum Type { STATIC, KINEMATIC, DYNAMIC };

		b2World& world_;
		b2Body* body_;
		Type type_;

	public:
		/**
		 * @param type Body type. Should be [ static, kinematic, dynamic ]
		 * @param position Position of this body in pixel coordinates
		*/
		RigidBody(Type type, const Point& position) : 
			world_(PhysicSystem::Get()->world_),
			type_(type)
		{
			b2BodyDef def;
			def.position.Set(pixelToMeter(position.x), pixelToMeter(position.y));
			def.type = (b2BodyType)type;

			body_ = world_.CreateBody(&def);
		}

		b2Body* getBody()
		{
			return body_;
		}

		~RigidBody()
		{
			auto tmp = getBody();
			world_.DestroyBody(body_);
		}

		void setDensity(float density)
		{
			for (auto fixture = body_->GetFixtureList(); fixture; fixture = fixture->GetNext())
				fixture->SetDensity(density);
		}

		void setGravityScale(float scale)
		{
			body_->SetGravityScale(scale);
		}

		void setAwake(bool awake)
		{
			body_->SetAwake(awake);
		}

		void setType(Type type)
		{
			body_->SetType((b2BodyType)type);
		}
	};

	class Collider
	{
		enum Type { CIRCLE, POLYGON, BOX };

		b2Fixture* fixture_;
		b2Body* body_;

		float density_;
		Type type_;

		void destroyFixture()
		{
			if (!body_)
				throw std::logic_error("No body attached to the entity!");

			if (!fixture_)
				return;

			body_->DestroyFixture(fixture_);
		}

	public:
		Collider(Type type, float density) : 
			type_(type),
			density_(density)
		{}

		b2Fixture* getFixture()
		{
			return fixture_;
		}

		void setDensity(float density)
		{
			density_ = density;
			fixture_->SetDensity(density);
		}

		// Set this shape to be a circle
		void setCircle(float radius)
		{
			b2CircleShape shape;
			shape.m_radius = radius;

			if (!body_)
				throw std::logic_error("No body attached to the entity!");

			destroyFixture();
			fixture_ = body_->CreateFixture(&shape, density_);
		}

		/**
		 * @brief Set this shape to be a polygon
		 * @param vertices List of points in pixel coordinates
		*/
		void setPolygon(const std::vector<Point>& vertices)
		{
			auto len = vertices.size();
			if (len > 8)
				throw std::logic_error("8 vertices are only allowed for a polygon shape");

			b2PolygonShape shape;
			for (int i = 0; i < len; ++i)
				shape.m_vertices[i].Set(pixelToMeter(vertices[i].x), pixelToMeter(vertices[i].y));

			shape.m_count = len;

			if (!body_)
				throw std::logic_error("No body attached to the entity!");

			destroyFixture();
			fixture_ = body_->CreateFixture(&shape, density_);
		}

		/**
		 * @brief Set this shape to be a box
		 * @param width Width of the box in pixels
		 * @param height height of the box in pixels
		*/
		void setBox(int width, int height)
		{
			b2PolygonShape shape;
			shape.SetAsBox(pixelToMeter(width * 0.5), pixelToMeter(height * 0.5));

			if (!body_)
				throw std::logic_error("No body attached to the entity!");

			destroyFixture();
			fixture_ = body_->CreateFixture(&shape, density_);
		}

		friend class Entity;
	};

}