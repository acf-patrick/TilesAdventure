#pragma once

#include <SDL2/SDL.h>
#include <box2d/box2d.h>

#include "core/geometry/geometry.h"

#define radToDeg(X) X * 180.0 / 3.14159265359

#define degToRad(X) X * 3.14159265359 / 180.0

#define meterPxFactor 80.0

#define pixelToMeter(X) X / meterPxFactor
#define meterToPixel(X) X *meterPxFactor

namespace ECS {
    class Collider;
    class RigidBody;
}

class PhysicSystem
{
private:
    b2World world_;

    const b2Vec2 gravity_;

    static PhysicSystem *instance;

    PhysicSystem();

    ~PhysicSystem();

public:
    static PhysicSystem *Get();

    /**
     * @brief Set gravity scale for game world
     * @param x scale along x axis
     * @param y scale along y axis
     */
    void setGravityScale(float x, float y);

    Vector getGravityScale() const;

    void update(Uint32 dt);

    friend class Game;
    friend class ECS::Collider;
    friend class ECS::RigidBody;
};