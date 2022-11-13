#include "core/ECS/entity.h"
#include "components.h"
#include "scripts/MoveCamera.h"
#include "scripts/Body.h"

void ECS::Entity::Deserialize(ECS::Entity* entity, YAML::Node node)
{
    if (node["MoveCamera"])
        entity->attachScript<MoveCamera>();

    if (node["Body"])
        entity->attachScript<Body>();

}