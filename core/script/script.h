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
            SDL_RenderCopy(renderer, sprite_->texture, &src, &dst);
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
}
