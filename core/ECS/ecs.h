#pragma once

#include <array>
#include <bitset>
#include <queue>
#include <memory>
#include <cstdint>
#include <unordered_map>

namespace ECS {

	// Type alias for entity ID
	using EntityID = std::uint32_t;

	// Maximum number of entities in the game
	const EntityID MAX_ENTITIES = 100;

	// Type alias for identification inside component array
	using ComponentID = std::uint8_t;

	// Maximum number of components that an entity can have
	const ComponentID MAX_COMPONENTS = 32;

	// Used to check if an entity has a specific component (ComponentID)
	using Signature = std::bitset<MAX_COMPONENTS>;

	class EntityManager
	{
	public:
		static EntityManager* Get();

		EntityID createEntity();

		void destroyEntity(EntityID entity);

		void setSignature(EntityID entity, ComponentID componentID, bool value);

		Signature getSignature(EntityID entity);

	private:
		static EntityManager* instance_;

		EntityManager();

		// Queue of unused entity IDs
		std::queue<EntityID> availableEntities_;

		// EntityID -> Signature
		std::array<Signature, MAX_ENTITIES> signatures_;

		// Total living entities
		EntityID livingEntityCount_ = 0;
	};

	class IComponentArray
	{
	public:
		virtual ~IComponentArray() = default;
		virtual void entityDestroyed(EntityID entity) = 0;
	};

	template<typename T>
	class ComponentArray : public IComponentArray
	{
	private:
		// EntityID -> Component data
		std::array<T*, MAX_ENTITIES> componentArray_;

		// Entity ID -> Index
		std::unordered_map<EntityID, size_t> entityToIndex_;

		// Index -> Entity ID
		std::unordered_map<size_t, EntityID> indexToEntity_;

		// Total size of valid entries in the array
		size_t size_ = 0;

	public:
		void insertData(EntityID entity, T* component)
		{
			if (entityToIndex_.find(entity) != entityToIndex_.end())
				throw std::out_of_range("Component added to same entity more than once.");

			// Put new entry at end and update the maps
			size_t newIndex = size_;
			entityToIndex_[entity] = newIndex;
			indexToEntity_[newIndex] = entity;
			componentArray_[newIndex] = component;
			++size_;
		}

		void removeData(EntityID entity)
		{
			if (entityToIndex_.find(entity) == entityToIndex_.end())
				throw std::out_of_range("Removing non-existent component.");

			// Copy element at end into deleted element's place to maintain density
			size_t indexOfRemovedEntity = entityToIndex_[entity];
			size_t indexOfLastElement = size_ - 1;
			delete componentArray_[indexOfRemovedEntity];
			componentArray_[indexOfRemovedEntity] = componentArray_[indexOfLastElement];

			// Update map to point ot moved spot
			EntityID entityOfLastElement = indexToEntity_[indexOfLastElement];
			entityToIndex_[entityOfLastElement] = indexOfRemovedEntity;
			indexToEntity_[indexOfRemovedEntity] = entityOfLastElement;

			entityToIndex_.erase(entity);
			indexToEntity_.erase(indexOfLastElement);

			--size_;
		}

		T* getData(EntityID entity)
		{
			if (entityToIndex_.find(entity) == entityToIndex_.end())
				throw std::logic_error("Retrieving non-existent component.");

			return componentArray_[entityToIndex_[entity]];
		}

		void entityDestroyed(EntityID entity) override
		{
			if (entityToIndex_.find(entity) != entityToIndex_.end())
				removeData(entity);
		}
	};

	class ComponentManager
	{
	private:
		// Component class name -> Component ID
		std::unordered_map<const char*, ComponentID> componentIDs_;

		// Component class name -> Component array
		std::unordered_map<const char*, std::shared_ptr<IComponentArray>> componentArrays_;

		// Next component ID to be assigned to the next registered component
		ComponentID nextComponentID_ = 0;

		static ComponentManager* instance_;

		ComponentManager();

	public:
		static ComponentManager* Get();

		template<typename T>
		void registerComponent()
		{
			const char* typeName = typeid(T).name();

			if (componentIDs_.find(typeName) != componentIDs_.end())
				return;

			componentIDs_[typeName] = nextComponentID_;

			componentArrays_[typeName] = std::make_shared<ComponentArray<T>>();

			++nextComponentID_;
		}

		template<typename T>
		ComponentID getComponentID()
		{
			const char* typeName = typeid(T).name();

			if (componentIDs_.find(typeName) == componentIDs_.end())
				throw std::logic_error("Component not registered before use.");

			return componentIDs_[typeName];
		}

		template<typename T>
		void addComponent(EntityID entity, T* component)
		{
			getComponentArray<T>()->insertData(entity, component);
		}

		template<typename T>
		void removeComponent(EntityID entity)
		{
			getComponentArray<T>()->removeData(entity);
		}

		template<typename T>
		T* getComponent(EntityID entity)
		{
			return getComponentArray<T>()->getData(entity);
		}

		void entityDestroyed(EntityID entity)
		{
			for (auto& pair : componentArrays_)
				pair.second->entityDestroyed(entity);
		}

	private:
		template<typename T>
		std::shared_ptr<ComponentArray<T>> getComponentArray()
		{
			const char* typeName = typeid(T).name();
			if (componentIDs_.find(typeName) == componentIDs_.end())
				throw std::logic_error("Component not registered before use.");

			return std::static_pointer_cast<ComponentArray<T>>(componentArrays_[typeName]);
		}
	};
}
