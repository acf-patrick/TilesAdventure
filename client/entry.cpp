#include "core/ECS/entity.h"

#include "scripts/Switch.h"

void ECS::Entity::Deserialize(ECS::Entity* entity, YAML::Node node)
{
    if (node["Switch"])
        entity->attachScript<Switch>();
}