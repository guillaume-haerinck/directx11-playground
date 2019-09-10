#pragma once

namespace scomp {
	enum ConstantBufferIndex {
		PER_MESH = 0,
		PER_FRAME,
		PER_LIGHT_CHANGE,
		PER_COOK_MAT_CHANGE,
		PER_PHONG_MAT_CHANGE,
		_CONST_BUFFER_MAX
	};

	// Stored here only for updates. The shaders store them only for bindings.
	struct ConstantBuffers {
		std::array<comp::ConstantBuffer, ConstantBufferIndex::_CONST_BUFFER_MAX> constantBuffers;
	};
}
