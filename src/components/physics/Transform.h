#pragma once

namespace comp {
	/**
	 * @brief Store position, rotation and scaling of an entity in the world
	 */
	struct Transform {
		Transform(XMFLOAT3 position = { 0, 0, 0 }, XMFLOAT4 rotation = { 0, 0, 0, 1}, XMFLOAT3 scale = { 1, 1, 1 }, XMFLOAT3 rotationEuler = { 0, 0, 0 })
			: position(position), rotation(rotation), scale(scale), rotationEuler(rotationEuler) {}

		XMFLOAT3 position;
		XMFLOAT4 rotation;
		XMFLOAT3 scale;
		XMFLOAT3 rotationEuler; // Temp value not used by renderer
	};
}
