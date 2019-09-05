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
	// TODO create and handle update of constant buffer for light arrays
	struct Lights {
		std::vector<PointLight> pointLights;
		std::vector<DirectionalLight> directionalLights;
		std::vector<SpotLight> spotLights;
	};
}
