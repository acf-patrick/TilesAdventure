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
        if (event->getKey(SDLK_UP) or event->getJoystickAxis(1) < 0)
            camera_->y -= speed_ * dt;

        if (event->getKey(SDLK_DOWN) or event->getJoystickAxis(1) > 0)
            camera_->y += speed_ * dt;
            
        if (event->getKey(SDLK_LEFT) or event->getJoystickAxis(0) < 0)
            camera_->x -= speed_ * dt;
            
        if (event->getKey(SDLK_RIGHT) or event->getJoystickAxis(0) > 0)
            camera_->x += speed_ * dt;

        if (event->getJoystickButton(5))
            Scene::Get()->finish();
    }
};
