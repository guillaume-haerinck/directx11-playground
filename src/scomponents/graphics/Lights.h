#pragma once

#include "scomponents/graphics/Shaders.h"

namespace scomp {
	/**
	 * @brief
	 */
	struct SpotLight {
		DX::XMFLOAT3 color = { 1, 1, 1 };
		float spotAngle = 0;
		DX::XMFLOAT3 position = { 0, 0, 0};
		float intensity = 1;
		DX::XMFLOAT3 direction = { 0, 0, 0 };
		float attenuationRadius = 0;
	};

	/**
	 * @brief
	 */
	struct PointLight {
		DX::XMFLOAT3 color = { 1, 1, 1 };;
		float intensity = 1;
		DX::XMFLOAT3 position = { 0, 0, 0};
		float attenuationRadius = 0;
	};

	/**
	 * @brief
	 */
	struct DirectionalLight {
		DX::XMFLOAT3 color = { 1, 1, 1 };;
		float intensity = 1;
		DX::XMFLOAT3 direction = { 0, 0, 0};
	};

	/**
	 * @brief Unique array of Lights for the scene
	 */
	struct Lights {
		std::vector<PointLight> pointLights;
		std::vector<DirectionalLight> directionalLights;
		std::vector<SpotLight> spotLights;
		bool hasToBeUpdated = false;
	};
}
