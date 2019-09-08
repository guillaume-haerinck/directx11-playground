#pragma once

/**
 * @brief Structure for constant buffers used by shaders and updated on CPU side in render system
 */
namespace cb {
	/**
	 * @brief
	 *
	 * @note Updated once by frame
	 */
	struct Camera {
		XMFLOAT4X4 matViewProj;
	};

	/**
	 * @brief
	 *
	 * @note Is sent as an array. Updated once by frame
	 */
	struct MeshVariable {
		XMFLOAT4X4 matModel;
		// unsigned int materialIndex;
	};

	/**
	 * @brief
	 *
	 * @note Is sent as an array. Updated when there is a change
	 */
	struct CookTorranceMaterial {
		XMFLOAT3 meshAutoColor;

		int baseColorIndex;
		XMFLOAT3 baseColorFactor;

		int normalIndex;
		float normalScale;

		int metalRoughIndex;
		float roughnessFactor;
		float metallicFactor;

		int aoIndex;
		float aoStrength;

		int emissiveIndex;
		XMFLOAT3 emissiveFactor;
	};

	/**
	 * @brief
	 *
	 * @note Is sent as an array. Updated when there is a change
	 */
	struct PhongMaterial {

	};

	/**
	 * @brief
	 *
	 * @note Is sent as an array. Updated when there is a change
	 */
	struct Light {
		XMFLOAT3 strength;
		float falloffStart;		// point and spot light only
		XMFLOAT3 direction;		// directional and spot light only
		float falloffEnd;		// point and spot light only
		XMFLOAT3 position;		// point light only
		float spotPower;		// spot light only
	};
}
