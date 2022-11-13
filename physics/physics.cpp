#include "physics.h"

PhysicSystem* PhysicSystem::instance = nullptr;

PhysicSystem* PhysicSystem::Get()
{
    if (!instance)
        instance = new PhysicSystem;

    return instance;
}

PhysicSystem::PhysicSystem() : 
    world_({ 0.0f, 9.8f }),
    gravity_({ 0.0f, 9.8f })
{
}

PhysicSystem::~PhysicSystem()
{
    instance = nullptr;
}

void PhysicSystem::setGravityScale(float x, float y)
{
    auto gravity = gravity_;
    gravity.x *= x;
    gravity.y *= y;
    world_.SetGravity(gravity);
}

Vector PhysicSystem::getGravityScale() const
{
    auto gravity = world_.GetGravity();
    return { (gravity_.x != 0.0f ? (gravity.x / gravity_.x) : 0.0f), (gravity_.y != 0.0f ? (gravity.y / gravity_.y) : 0.0f) };
}

void PhysicSystem::update(Uint32 dt)
{
    world_.Step(dt / 1000.0f, 6, 2);
}