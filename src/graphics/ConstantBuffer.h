#pragma once

/**
 * @brief Structure for constant buffers used by shaders and updated on CPU side in render system
 * @note Sorted and named by their frequency of updates. Their bytesize is a multiple of 16.
 *		 char = 1 / float = 4 / uint32_t = 4 / XMFLOAT3 = 12 / XMFLOAT4X4 = 64
 *		 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 256, ...
 *
 * @link http://vzout.com/directx12/const-buffer-performance.html
 */
namespace cb {
	/**
	 * @brief Constant buffer updated for each mesh between draw calls (so many times a frame)
	 */
	struct perMesh {
		XMFLOAT4X4 matModel;
		uint32_t materialIndex;
		char padding[12];
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

		uint32_t baseColorIndex;
		XMFLOAT3 baseColorFactor;

		uint32_t normalIndex;
		float normalScale;

		uint32_t metalRoughIndex;
		float roughnessFactor;
		float metallicFactor;

		uint32_t aoIndex;
		float aoStrength;

		uint32_t emissiveIndex;
		XMFLOAT3 emissiveFactor;
	};

	/**
	 * @brief Constant buffer updated when a Phong material is changed
	 * @note Is sent as an array.
	 */
	struct perPhongMaterialChange {
		XMFLOAT3 ambientFactor;

		XMFLOAT3 diffuseFactor;
		uint32_t diffuseIndex;

		XMFLOAT3 specularFactor;
		uint32_t specularIndex;

		float shininess;
	};

	/**
	 * @brief Constant buffer updated when a light is changed
	 * @note Is sent as an array.
	 */
	struct perLightChange {
		XMFLOAT3 color;
		float intensity;
		XMFLOAT3 position;
		float spotAngle;
		XMFLOAT3 direction;
		float attenuationRadius;
	};
}
