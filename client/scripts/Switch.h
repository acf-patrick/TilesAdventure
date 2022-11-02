#pragma once

#include "core/script/script.h"

class Switch : public Script
{
    bool firstClic = true;

public:
    void update(Uint32 dt) override
    {
        auto event = EventHandler::Get();
        if (firstClic and event->getMouseButton(1))
        {
            firstClic = false;
            auto scene = Scene::Get();
            scene->load("Switch");
        }
    }
};