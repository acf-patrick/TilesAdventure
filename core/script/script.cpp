#include <algorithm>

#include "script.h"

Script::list Script::instances;

void Script::toggle(bool enable)
{
	if (enable == enabled_)
		return;

	if (enable)
		onEnable();
	else
		onDisable();

	enable = enabled_;
}

bool Script::isEnabled() const
{
	return enabled_;
}

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

void Script::onEnable()
{
	// Override
}

void Script::onDisable()
{
	// OVerride
}