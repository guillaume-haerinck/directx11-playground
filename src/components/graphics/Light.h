#pragma once

namespace comp {

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
	 * @brief
	 */
	struct Lights {
		std::vector<PointLight> pointLights;
		std::vector<DirectionalLight> directionalLights;
		std::vector<SpotLight> spotLights;
	};
}
