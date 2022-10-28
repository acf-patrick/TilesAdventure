#include <algorithm>

#include "script.h"

Script::list Script::instances;

Script::~Script()
{
	onRemove();
	instances.erase(this);
}

void Script::onAttach()
{
	// Override
}

void Script::onRemove()
{
	// Override
}

void Script::update(Uint32 dt)
{
	// Override
}

void Script::render(SDL_Renderer* renderer)
{
	// Override
}
