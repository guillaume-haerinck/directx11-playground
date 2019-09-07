#pragma once

#include "components/graphics/Shader.h"

namespace scomp {
	struct Camera {
		XMFLOAT4X4 view;
		XMFLOAT4X4 proj;
		comp::ConstantBuffer constantBuffer;
	};
}
