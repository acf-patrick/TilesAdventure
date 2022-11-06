#pragma once

#include "./base.h"

#include <functional>

namespace ECS {
    class SpriteRenderer : public Script
	{
        Sprite* sprite_;
        Transform *transform_;

    public:
        void onAttach() override
        {
            auto& sprite = getComponent<Sprite>();
            sprite_ = &sprite;
			auto &transform = getComponent<Transform>();
			transform_ = &transform;
        }

        void render(SDL_Renderer* renderer) override
        {
            SDL_Rect camera;
            SDL_GetRendererOutputSize(renderer, &camera.w, &camera.h);

            auto& cameraTransform = Scene::Get()->getEntity("camera")->getComponent<Transform>();
            camera.x = cameraTransform.position.x;
            camera.y = cameraTransform.position.y;
            camera.w *= cameraTransform.scale.x;
            camera.h *= cameraTransform.scale.y;

			SDL_Rect src;
            if (sprite_->sliced)
            {
                int width = sprite_->source.w / sprite_->slice.x;
                int x = sprite_->index % width;
                int y = sprite_->index / width;
                src = { x * sprite_->slice.x, y * sprite_->slice.y, sprite_->slice.x, sprite_->slice.y };
                src.x += sprite_->margin.x + x * sprite_->spacing.x;
                src.y += sprite_->margin.y + y * sprite_->spacing.y;
            }
            else src = sprite_->source;
            SDL_Rect dst = { transform_->position.x, transform_->position.y, src.w * transform_->scale.x, src.h * transform_->scale.y };

            SDL_Rect tmp;
            if (SDL_IntersectRect(&camera, &dst, &tmp))
            {
                dst.x -= camera.x;
                dst.y -= camera.y;
                SDL_RenderCopy(renderer, sprite_->texture, &src, &dst);
            }
        }
    };

    class SpriteAnimator : public Script
    {
        using Callback = std::function<void()>;
        std::map<int, std::vector<Callback>> listeners_;

        Uint32 elapsed_ = 0;
        Sprite* sprite_;

    public:
		// Sprite index to draw from Sprite component
		std::vector<int> frames;

		// Frame per second
		int rate = 1;

    public:
		SpriteAnimator(const std::vector<int>& frameList) :
            frames(frameList)
        {}

        // Use all frames in spritesheet by default
        SpriteAnimator() = default;

        void onAttach() override
        {
            Sprite& sprite = getComponent<Sprite>();
            sprite_ = &sprite;

            // Computes all frames
            if (frames.empty() and sprite.sliced)
            {
                SDL_Point total = { (sprite.source.w - sprite.source.x) / sprite.slice.x, (sprite.source.h - sprite.source.y) / sprite.slice.y };
                for (int i = 0; i < total.x * total.y; ++i)
                    frames.emplace_back(i);
            }
        }

        void update(Uint32 dt) override
        {
            if (!sprite_->sliced)
                return;

            elapsed_ += dt;
            auto frameCount = frames.size();
            auto currentFrame = sprite_->index;
            if (elapsed_ > 1000.0 / rate)
            {
                elapsed_ = 0;
                currentFrame = (currentFrame + 1) % frameCount;

                auto& listeners = listeners_[currentFrame];
                for (auto listener : listeners)
                    listener();
            }
            sprite_->index = currentFrame;
        }

        void addListener(int frame, const Callback& callback)
        {
            listeners_[frame].emplace_back(callback);
        }
    };

    class TilemapRenderer : public Script
    {
    protected:
        TextureManager* texture_;
        Transform* transform_;
        Transform* camera_;
        tson::Map* map_;

        virtual void drawObject(tson::Object& object, SDL_Renderer* renderer)
        {
            // override
        }

        void drawLayer(tson::Layer& layer, SDL_Renderer* renderer)
        {
            auto type = layer.getType();
            if (type == tson::LayerType::Group)
                for (auto& lay: layer.getLayers())
                    drawLayer(lay, renderer);
            else if (type == tson::LayerType::ImageLayer)
            {
                auto image = layer.getImage();
                image = image.substr(10);
                auto texture = texture_->get(texture_->createTag(image));
                if (!texture)
                    return;
                auto offset = layer.getOffset();
                SDL_Rect dst = { offset.x - camera_->position.x, offset.y - camera_->position.y };
                SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
                SDL_RenderCopy(renderer, texture, NULL, &dst);
            }
            else if (type == tson::LayerType::ObjectGroup)
            {
                for (auto& object : layer.getObjects())
                    drawObject(object, renderer);
            }
            else if (type == tson::LayerType::TileLayer)
            {
                SDL_Rect camera = { camera_->position.x, camera_->position.y };
                auto tileSize = map_->getTileSize();
                SDL_GetRendererOutputSize(renderer, &camera.w, &camera.h);
                camera.w *= camera_->scale.x;
                camera.h *= camera_->scale.y;
                
                SDL_Point min = { camera.x / tileSize.x - 1, camera.y / tileSize.y - 1 };
                SDL_Point max = { min.x + (camera.w / tileSize.x) + 1, min.y + (camera.h / tileSize.y) + 1 };

                auto mapSize = map_->getSize();
                for (int x = min.x; x <= max.x; ++x)
                {
                    for (int y = min.y; y <= max.y; ++y)
                    {
                        if (x < 0 or y < 0 or x >= mapSize.x or y >= mapSize.y)
                            continue;

                        auto tile = layer.getTileData(x, y);
                        if (tile)
                        {
                            auto tileset = tile->getTileset();
                            auto image = tileset->getImage().string();
                            image = image.substr(10);
                            auto texture = texture_->get(texture_->createTag(image));
                            auto rect = tile->getDrawingRect();
                            SDL_Rect 
                                src = { rect.x, rect.y, rect.width, rect.height },
                                dst = { x * tileSize.x - camera.x, y * tileSize.y - camera.y, rect.width, rect.height };
                            SDL_RenderCopy(renderer, texture, &src, &dst);
                        }
                    }
                }
            }
        }

    public:
        void onAttach() override
        {
            texture_ = TextureManager::Get();

            auto& transform = getComponent<Transform>();
            transform_ = &transform;

            auto& camera = Scene::Get()->getEntity("camera")->getComponent<Transform>();
            camera_ = &camera;

            auto& map = getComponent<Tilemap>();
            map_ = map.map_.get();
        }

        void render(SDL_Renderer* renderer) override
        {
            auto& layers = map_->getLayers();
            for (auto &layer : layers)
                drawLayer(layer, renderer);
        }
    };
}
