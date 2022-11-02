#pragma once

#include <map>
#include <iostream>
#include <SDL2/SDL.h>
#include <yaml-cpp/yaml.h>

#include "core/const.h"
#include "core/ECS/component.h"
#include "core/ECS/entity.h"

class Scene 
{
private:
    // Name of the scene
    std::string name_;
        
    // All entities in the scene
    std::map<std::string, ECS::Entity*> entities_;
        
    std::string loadRequest_; 

    //  The instance of the scene
    static Scene* instance;

private:
    void load_(const std::string& name);

    Scene();
		
public:

    ~Scene();
        
    // Update and render scene
    void render(SDL_Renderer*);

    void update( Uint32 dt );
        
    // Getting all entities in the scene
    ECS::Entity* getEntity(const std::string& tag);

	// Getting the scene instance
    static Scene* Get();
        
    // Load new scene
    void load( const std::string& name );
        
    friend class Game;
};