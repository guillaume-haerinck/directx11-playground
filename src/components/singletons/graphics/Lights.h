#pragma once

#include "components/graphics/Shader.h"

namespace scomp {
	/**
	 * @brief
	 */
	struct SpotLight {
		DX::XMFLOAT3 color;
		float spotAngle;
		DX::XMFLOAT3 position;
		float intensity;
		DX::XMFLOAT3 direction;
		float attenuationRadius;
	};

	/**
	 * @brief
	 */
	struct PointLight {
		DX::XMFLOAT3 color;
		float intensity;
		DX::XMFLOAT3 position;
		float attenuationRadius;
	};

	/**
	 * @brief
	 */
	struct DirectionalLight {
		DX::XMFLOAT3 color;
		float intensity;
		DX::XMFLOAT3 direction;
	};

	/**
	 * @brief Unique array of Lights for the scene
	 */
	struct Lights {
		Lights() : hasToBeUpdated(false) {};

		std::vector<PointLight> pointLights;
		std::vector<DirectionalLight> directionalLights;
		std::vector<SpotLight> spotLights;
		bool hasToBeUpdated;
	};
}
