#pragma once

#include "components/graphics/Shader.h"

namespace scomp {

	/**
	 * @brief
	 */
	struct PointLight {
		XMFLOAT3 position;
	};

	/**
	 * @brief
	 */
	struct DirectionalLight {
	};

	/**
	 * @brief
	 */
	struct SpotLight {
		XMFLOAT3 position;
	};

	/**
	 * @brief Unique array of Lights for the scene
	 */
	struct Lights {
		std::vector<PointLight> pointLights;
		std::vector<DirectionalLight> directionalLights;
		std::vector<SpotLight> spotLights;
	};
}
