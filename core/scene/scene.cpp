#include "core/scene/scene.h"
#include "core/script/script.h"

Scene* Scene::instance = nullptr;

Scene* Scene::Get()
{
    if( !instance )
        new Scene;
        
    return instance;
}

Scene::Scene()
{
    instance = this;
}

Scene::~Scene()
{
	clear();
}

void Scene::render(SDL_Renderer* renderer)
{
	for (auto script : Script::instances)
		if (script->isEnabled())
			script->render(renderer);
}

void Scene::update( Uint32 dt )
{
	for (auto script : Script::instances)
		if (script->isEnabled())
			script->update(dt);

	if (!loadRequest_.empty())
		load_(loadRequest_);
}

ECS::Entity* Scene::getEntity(const std::string& tag)
{
    return entities_[tag];
}

void Scene::load( const std::string& name )
{
	// No request pending
	if (loadRequest_.empty())
		loadRequest_ = name;
}

void Scene::clear()
{
    for( auto& i : entities_ )
        delete i.second;
	entities_.clear();
}

void Scene::load_(const std::string& name)
{
    name_ = name;

	clear();

    //  Loading YAML
    YAML::Node entities = YAML::LoadFile( SCENES_PATH + name + ".scn" );
    
    if( !entities )
        throw std::logic_error( name + " : Scene not found!" );
    
	if (entities)
	{
		for( auto entity : entities )
		{
			auto prefab = entity["prefab"];
			std::string tag;
			ECS::Entity* e = new ECS::Entity;

			if (prefab)
			{
				auto file = PREFABS_PATH + prefab.as<std::string>() + ".ent";
				YAML::Node node = YAML::LoadFile(file);
				if (node)
				{
					if (node["tag"])
						tag = node["tag"].as<std::string>();
					else
						tag = std::to_string(e->getID());
					ECS::Entity::Load(e, node);
				}
				else 
					throw std::logic_error("Prefab file for " + prefab.as<std::string>() + " not found!");
			}

			if (entity["tag"])
				tag = entity["tag"].as<std::string>();
			else
				tag = std::to_string(e->getID());

			if (entities_.find(tag) != entities_.end())
				tag += "#" + std::to_string(e->getID());
			
			entities_[tag] = e;

			ECS::Entity::Load(e, entity);
		}
	}

	std::cout << "Scene " << name << " successfully loaded" << std::endl;
	
	// Free queue
	loadRequest_.clear();
}