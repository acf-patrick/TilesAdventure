#include "core/script/script.h"

#include <box2d/box2d.h>


#define radToDeg(X) X * 180.0 / 3.14159265359

#define degToRad(X) X * 3.14159265359 / 180.0

#define meterPxFactor 80.0

#define pixelToMeter(X) X / meterPxFactor
#define meterToPixel(X) X * meterPxFactor

class Body : public Script 
{
    SDL_Texture* texture_;
    b2World world_;
    b2Body* body_;
    b2Body* ground_;

    void createGround()
    {
        b2BodyDef bodyDef;
        bodyDef.type = b2_staticBody;
        bodyDef.position.Set(pixelToMeter(200), pixelToMeter(200));
        
        ground_ = world_.CreateBody(&bodyDef);
        b2PolygonShape box;
        box.SetAsBox(pixelToMeter(50), pixelToMeter(5));

        b2EdgeShape edge;
        edge.SetTwoSided({ pixelToMeter(-50), 0 }, { pixelToMeter(50), 0 });

        ground_->CreateFixture(&edge, 0.0f);

    }

public:
    Body() : world_({ 0.0f, 9.8f })
    {}

    void onAttach() override
    {
        texture_ = TextureManager::Get()->get("Main_Characters_Virtual_Guy_Idle");

        createGround();

        b2BodyDef bodyDef;
        bodyDef.type = b2_dynamicBody;
        bodyDef.angle = degToRad(60);
        bodyDef.position.Set(pixelToMeter(200), 0.0f);

        body_ = world_.CreateBody(&bodyDef);

        b2PolygonShape box;
        box.SetAsBox(pixelToMeter(16), pixelToMeter(16));

        b2FixtureDef fixtureDef;
        fixtureDef.shape = &box;
        fixtureDef.density = 2.0f;
        fixtureDef.restitution = 0.5;
        body_->CreateFixture(&fixtureDef);
    }

    void update(Uint32 dt) override
    {
        // world_.Step(dt / 1000.0f, 6, 2);
    }

    void render(SDL_Renderer* renderer) override
    {
        return;
        auto angle = body_->GetAngle();
        auto position = body_->GetPosition();
        SDL_Rect 
            src = { 0, 0, 32, 32 },
            dst = { meterToPixel(position.x) - 16, meterToPixel(position.y) - 16, 32, 32 };
        SDL_RenderCopyEx(renderer, texture_, &src, &dst, radToDeg(angle), NULL, SDL_FLIP_NONE);

        position = ground_->GetPosition();

        SDL_Rect ground = { meterToPixel(position.x) - 50, meterToPixel(position.y) - 5, 100, 10 };
        SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);
        SDL_RenderDrawRect(renderer, &ground);
    }
};
