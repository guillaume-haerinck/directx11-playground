#pragma once

#include "graphics/RenderCommand.h"

enum SingletonComponents {
	GRAPHIC = 0,
	IO,
	_SINGLETON_COMP_MAX
};

struct Context {
	Context() {
		singletonComponents.fill(entt::null);
	}

	std::unique_ptr<RenderCommand> rcommand;
	entt::registry registry;
	std::array<entt::entity, _SINGLETON_COMP_MAX> singletonComponents;
};
