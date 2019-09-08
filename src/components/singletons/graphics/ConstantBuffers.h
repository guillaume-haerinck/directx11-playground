#pragma once

namespace scomp {
	enum ConstantBufferIndex {
		CAMERA = 0,
		MESH_VARIABLES,
		LIGHTS,
		COOK_TORRANCE_MATERIALS,
		PHONG_MATERIALS,
		CONST_BUFFER_MAX
	};

	// Stored here only for updates. The shaders store them only for bindings.
	struct ConstantBuffers {
		std::array<comp::ConstantBuffer, ConstantBufferIndex::CONST_BUFFER_MAX> constantBuffers;
	};
}
