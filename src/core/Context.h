#pragma once

#include "graphics/RenderCommand.h"

enum SingletonComponents {
	GRAPHIC = 0,
	IO,
	_SINGLETON_COMP_MAX
};

struct Context {
	std::unique_ptr<RenderCommand> rcommand;
	entt::registry registry;
	std::array<unsigned int, _SINGLETON_COMP_MAX> singletonComponents;
};
