#pragma once


#include <set>
#include <vector>
#include <SDL2/SDL.h>

#include "core/ECS/entity.h"
#include "core/scene/scene.h"
#include "core/ECS/component.h"

#include "event/event.h"

// Base class for script objects
class Script 
{
protected:
    ECS::Entity* entity_;

    bool enabled_ = true;

public:
    struct compare
    {
        bool operator()( const Script* s1, const Script* s2 ) const
        {
            auto e1 = s1->entity_;
            auto e2 = s2->entity_;
            size_t
            z1 = e1->getComponent<ECS::Transform>().zIndex,
            z2 = e2->getComponent<ECS::Transform>().zIndex;
            return z1 < z2;
        }
    };
    using list = std::multiset<Script*, compare>;

    virtual ~Script();

    // Called when script is about to be attached
    virtual void onAttach();

    // Called when script is about to be removed from entity
    virtual void onRemove();

    // Called when script is toggled
    virtual void onEnable();

    // Called when script is disabled
    virtual void onDisable();

    /**
     * @brief Called between each frame
     * @param dt Delta time between each call in milliseconds
    */
    virtual void update( Uint32 dt );

    virtual void render( SDL_Renderer* );

private:
    static list instances;

public:
    // Toggle this script
    void toggle(bool enable = true);

    bool isEnabled() const;

    template<typename T>
    T& getComponent()
    {
        return entity_->getComponent<T>();
    }

    template<typename T>
    void remove()
    {
        entity_->remove<T>();
    }

    template<typename T, typename... TArgs>
    T& attachScript( TArgs&& ... args )
    {
        return entity_->attachScript<T>( std::forward<TArgs>( args )... );
    }

    template<typename T, typename... TArgs>
    T& attach( TArgs&& ... args )
    {
        return entity_->attach<T>( std::forward<TArgs>( args )... );
    }

    template<typename T>
    bool has() const
    {
        return entity_->has<T>();
    }

    template<typename... T>
    bool hasAllOf() const
    {
        return entity_->hasAllOf<T...>();
    }

    template<typename... T>
    bool hasNoneOf() const
    {
        return entity_->hasNoneOf<T...>();
    }

    template<typename... T>
    bool hasAnyOf() const
    {
        return entity_->hasAnyOf<T...>();
    }

    friend struct compare;
    friend class ECS::Entity;
    friend class Scene;
};