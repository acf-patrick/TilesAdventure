#include "core/ECS/entity.h"
#include "components.h"
#include "scripts/MyScript.h"
#include "scripts/MyScript.h"

void ECS::Entity::Deserialize(ECS::Entity* entity, YAML::Node node)
{
    if (node["Component"])
        entity->attach<Component>();

    if (node["MyScript"])
        entity->attachScript<MyScript>();
}