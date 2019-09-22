#pragma once

namespace scomp {
	/**
	 * @brief Constant buffers used by shader. This structure is used to update constant buffers.
	 */
	struct ConstantBuffer {
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
		unsigned int byteWidth = 0;
		// TODO keep track of the layout
	};

	/**
	 * @note The layouts are accessible on the graphics/ConstantBuffer.h file
	 */
	enum ConstantBufferIndex {
		PER_MESH = 0,
		PER_FRAME,
		PER_LIGHT_CHANGE,
		PER_COOK_MAT_CHANGE,
		PER_PHONG_MAT_CHANGE,
		PER_CUSTOM_PROP_CHANGE, // Updated by example
		_CONST_BUFFER_MAX
	};

	// Stored here only for updates. The shaders store them only for bindings.
	struct ConstantBuffers {
		std::array<ConstantBuffer, ConstantBufferIndex::_CONST_BUFFER_MAX> constantBuffers;
	};
}
