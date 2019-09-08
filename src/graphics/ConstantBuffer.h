#pragma once

/**
 * @brief Structure for constant buffers used by shaders and updated on CPU side in render system
 * @note Sorted and nammed by their frequency of updates.
 *
 * @link http://vzout.com/directx12/const-buffer-performance.html
 */
namespace cb {
	/**
	 * @brief Constant buffer updated for each mesh between draw calls (so many times a frame)
	 */
	struct perMesh {
		XMFLOAT4X4 matModel;
		// unsigned int materialIndex;
	};

	/**
	 * @brief Constant buffer updated once per frame
	 */
	struct perFrame {
		XMFLOAT4X4 matViewProj;
	};

	/**
	 * @brief Constant buffer updated when a PBR material is changed
	 * @note Is sent as an array.
	 */
	struct perCookTorranceMaterialChange {
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
	 * @brief Constant buffer updated when a Phong material is changed
	 * @note Is sent as an array.
	 */
	struct perPhongMaterialChange {

	};

	/**
	 * @brief Constant buffer updated when a light is changed
	 * @note Is sent as an array.
	 */
	struct perLightChange {
		XMFLOAT3 strength;
		float falloffStart;		// point and spot light only
		XMFLOAT3 direction;		// directional and spot light only
		float falloffEnd;		// point and spot light only
		XMFLOAT3 position;		// point light only
		float spotPower;		// spot light only
	};
}
