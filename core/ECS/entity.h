#pragma once

#include "ecs.h"
#include <yaml-cpp/yaml.h>

namespace ECS
{
	class Entity
	{
	private:
		EntityID id_;

		EntityManager *entityManager_;

		ComponentManager *componentManager_;

	public:
		Entity();

		Entity(const Entity&) = delete;

		Entity(Entity&&) = delete;

		~Entity();

		static void Load(Entity *, YAML::Node);

		static void Deserialize(Entity *, YAML::Node);

		EntityID getID() const
		{
			return id_;
		}

		// Disallow copying
		Entity &operator=(const Entity &) = delete;

		// Check if entity has specified component
		template <typename T>
		bool has() const
		{
			auto signature = entityManager_->getSignature(id_);
			auto componentID = componentManager_->getComponentID<T>();
			return signature[componentID];
		}

		// Check if entity has all of components given in template
		template <typename... T>
		bool hasAllOf() const
		{
			return (has<T>() && ...);
		}

		// Check if entity has none of the components given in template
		template <typename... T>
		bool hasNoneOf() const
		{
			return ((!has<T>()) && ...);
		}

		// Check if entity has any of the components given in template
		template <typename... T>
		bool hasAnyOf() const
		{
			return (has<T>() || ...);
		}

		template <typename T, typename... TArgs>
		T &attach(TArgs &&...args)
		{
			T *ret = new T(std::forward<TArgs>(args)...);
			componentManager_->registerComponent<T>();
			componentManager_->addComponent<T>(id_, ret);
			entityManager_->setSignature(id_, componentManager_->getComponentID<T>(), true);
			return *ret;
		}

		template <typename T, typename... TArgs>
		T &attachScript(TArgs &&...args)
		{
			T &ret = attach<T>(std::forward<TArgs>(args)...);
			ret.entity_ = this;
			ret.instances.insert(&ret);
			ret.onAttach();
			return ret;
		}

		template <typename T>
		void remove()
		{
			componentManager_->removeComponent<T>(id_);
			entityManager_->setSignature(id_, componentManager_->getComponentID<T>(), false);
		}

		template <typename T>
		T &getComponent()
		{
			T *ret = componentManager_->getComponent<T>(id_);
			return *ret;
		}
	};
}
