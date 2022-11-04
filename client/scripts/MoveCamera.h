#include "event/event.h"
#include "core/script/script.h"
#include "core/geometry/geometry.h"

class MoveCamera : public Script 
{
    Point* camera_;
    float speed_ = 0.5f;

public:
    void onAttach() override
    {
        auto& position = getComponent<ECS::Transform>().position;
        camera_ = &position;
    }

    void update(Uint32 dt) override
    {
        auto event = EventHandler::Get();
        if (event->getKey(SDLK_UP))
            camera_->y -= speed_ * dt;

        if (event->getKey(SDLK_DOWN))
            camera_->y += speed_ * dt;
            
        if (event->getKey(SDLK_LEFT))
            camera_->x -= speed_ * dt;
            
        if (event->getKey(SDLK_RIGHT))
            camera_->x += speed_ * dt;
    }
};     
