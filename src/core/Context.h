#pragma once

#include "graphics/RenderCommand.h"

struct Context {
	std::unique_ptr<RenderCommand> rcommand;
	entt::registry registry;
};
