#include "ecs.h"
#include "entity.h"
#include "component.h"
#include "core/script/script.h"

namespace ECS
{

	EntityManager *EntityManager::instance_ = nullptr;

	EntityManager::EntityManager()
	{
		if (!instance_)
			instance_ = this;

		// Initialize the queue with all possible entity IDs
		for (EntityID entity = 0; entity < MAX_ENTITIES; ++entity)
			availableEntities_.push(entity);
	}

	EntityID EntityManager::createEntity()
	{
		if (livingEntityCount_ > MAX_ENTITIES)
			throw std::out_of_range("Too many entities in existence.");

		// Take an ID from the front of the queue
		EntityID id = availableEntities_.front();
		availableEntities_.pop();
		++livingEntityCount_;

		return id;
	}

	void EntityManager::destroyEntity(EntityID entity)
	{
		if (entity > MAX_ENTITIES)
			throw std::out_of_range("Entity out of range.");

		// Invalidate the destroyed entity's signature
		signatures_[entity].reset();

		availableEntities_.push(entity);
		--livingEntityCount_;
	}

	void EntityManager::setSignature(EntityID entity, ComponentID componentID, bool value)
	{
		if (entity > MAX_ENTITIES || componentID > MAX_COMPONENTS)
			throw std::out_of_range("Entity out of range");

		// Put this entity's signature into the array
		signatures_[entity].set(componentID, value);
	}

	Signature EntityManager::getSignature(EntityID entity)
	{
		if (entity > MAX_ENTITIES)
			throw std::out_of_range("Entity out of range");

		// Get this entity signature from the array
		return signatures_[entity];
	}

	EntityManager *EntityManager::Get()
	{
		if (!instance_)
			new EntityManager;
		return instance_;
	}

	ComponentManager *ComponentManager::instance_ = nullptr;

	ComponentManager::ComponentManager()
	{
		if (!instance_)
			instance_ = this;
	}

	ComponentManager *ComponentManager::Get()
	{
		if (!instance_)
			new ComponentManager;
		return instance_;
	}

	Entity::Entity()
	{
		entityManager_ = EntityManager::Get();
		componentManager_ = ComponentManager::Get();
		id_ = entityManager_->createEntity();

		// Entities have Transform component by default
		attach<Transform>();
	}

	Entity::~Entity()
	{
		entityManager_->destroyEntity(id_);
		componentManager_->entityDestroyed(id_);
	}

	void Entity::Load(Entity *e, YAML::Node node)
	{
		// Load components
		{
			// Getting transform
			auto nTransform = node["Transform"];
			if (nTransform)
			{
				auto &transform = e->getComponent<ECS::Transform>();
				if (nTransform["position"])
				{
					auto pos = nTransform["position"].as<std::vector<double>>();
					transform.position.x = pos[0];
					transform.position.y = pos[1];
				}
				if (nTransform["scale"])
				{
					auto scale = nTransform["scale"].as<std::vector<double>>();
					transform.scale.x = scale[0];
					transform.scale.y = scale[1];
				}
				if (nTransform["rotation"])
					transform.rotation = nTransform["rotation"].as<double>();
				if (nTransform["zIndex"])
					transform.zIndex = nTransform["zIndex"].as<int>();
			}

			// Getting sprite
			auto nSprite = node["Sprite"];
			if (nSprite)
			{
				SDL_Color color;
				bool useColorkey = false;
				if (nSprite["colorkey"])
				{
					useColorkey = true;
					auto colorkey = nSprite["colorkey"].as<std::vector<int>>();
					color.r = colorkey[0];
					color.g = colorkey[1];
					color.b = colorkey[2];
				}

				auto t = nSprite["texture"].as<std::string>();
				auto &sprite = useColorkey ? e->attach<ECS::Sprite>(t, color) : e->attach<ECS::Sprite>(t);

				if (nSprite["source"])
				{
					auto src = nSprite["source"].as<std::vector<int>>();
					sprite.source = {src[0], src[1], src[2], src[3]};
				}

				if (nSprite["slice"])
				{
					sprite.sliced = true;
					auto slices = nSprite["slice"].as<std::vector<int>>();
					sprite.slice = {slices[0], slices[1]};
				}

				if (nSprite["index"])
					sprite.index = nSprite["index"].as<int>();

				if (nSprite["spacing"])
				{
					auto tmp = nSprite["spacing"].as<std::vector<int>>();
					sprite.spacing.x = tmp[0];
					sprite.spacing.y = tmp[1];
				}

				if (nSprite["margin"])
				{
					auto tmp = nSprite["margin"].as<std::vector<int>>();
					sprite.margin.x = tmp[0];
					sprite.margin.y = tmp[1];
				}
			}
		}

		// Load scripts
		{
			auto n = node["SpriteRenderer"];
			if (n)
				e->attachScript<SpriteRenderer>();

			n = node["SpriteAnimator"];
			if (n)
			{
				auto frames = n["frames"];
				auto &animator = frames ? e->attachScript<SpriteAnimator>(frames.as<std::vector<int>>()) : e->attachScript<SpriteAnimator>();
				animator.rate = n["rate"].as<int>();
			}
		}

		// User loader
		Deserialize(e, node);
	}
}
