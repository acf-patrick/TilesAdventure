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
        
    // Scene current state
    bool running_ = true;

    // Used when loading new scene
    std::string loadRequest_; 

    //  The instance of the scene
    static Scene* instance;

private:
    void load_(const std::string& name);

    void clear();

    Scene();
		
    ~Scene();
        
    void render(SDL_Renderer*);

    void update( Uint32 dt );

public:

    /**
     * @brief Terminate this scene
     * If no load request has been made, then application will finish
     */
    void finish();
    
    // Get all entities within the scene
    ECS::Entity* getEntity(const std::string& tag);

	// Get current Scene
    static Scene* Get();
        
    /**
     * @brief Load new scene
     * @param name Name of the scene to load (Should be in ./Assets/Scenes/)
     * @param important Indicates if this load request is important than any previous request made
    */
    void load( const std::string& name, bool important = false);
        
    friend class Game;
};