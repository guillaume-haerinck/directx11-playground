#pragma once

namespace comp {
	/**
	 * @brief Store position, rotation and scaling of an entity in the world
	 */
	struct Transform {
		Transform(DX::XMFLOAT3 position = { 0, 0, 0 }, DX::XMFLOAT4 rotation = { 0, 0, 0, 1}, DX::XMFLOAT3 scale = { 1, 1, 1 }, DX::XMFLOAT3 rotationEuler = { 0, 0, 0 })
			: position(position), rotation(rotation), scale(scale), rotationEuler(rotationEuler) {}

		DX::XMFLOAT3 position;
		DX::XMFLOAT4 rotation;
		DX::XMFLOAT3 scale;
		DX::XMFLOAT3 rotationEuler; // Temp value not used by renderer
	};
}
