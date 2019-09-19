#pragma once

namespace comp {
	/**
	 * @brief Store position, rotation and scaling of an entity in the world
	 */
	struct Transform {
		DX::XMFLOAT4 rotation = { 0, 0, 0, 1 };
		DX::XMFLOAT3 position = { 0, 0, 0 };
		float scale = 1;
		DX::XMFLOAT3 rotationEuler = { 0, 0, 0 };	// TODO remove me
	};
}
