#pragma once

/**
 * @brief Structure for constant buffers used by shaders and updated on CPU side in render system
 * @note Sorted and named by their frequency of updates. Their bytesize is a multiple of 16.
 *		 char = 1 / float = 4 / uint32_t = 4 / DX::XMFLOAT3 = 12 / DX::XMFLOAT4X4 = 64
 *		 16, 32, 48, 64, 80, 96, 112, 128, 144, 160, 176, 192, 208, 224, 240, 256, ...
 *
 * @link http://vzout.com/directx12/const-buffer-performance.html
 */
namespace cb {
	/**
	 * @brief Constant buffer updated for each mesh between draw calls (so many times a frame)
	 */
	struct perMesh {
		DX::XMFLOAT4X4 matModel;
		uint32_t materialIndex;
		char padding[12];
	};

	/**
	 * @brief Constant buffer updated once per frame
	 */
	struct perFrame {
		DX::XMFLOAT4X4 matViewProj;
		DX::XMFLOAT3 cameraPos;
		char padding[4];
	};

	/**
	 * @brief Constant buffer updated when a PBR material is changed
	 * @note Is sent as an array.
	 */
	struct perCookTorranceMaterialChange {
		DX::XMFLOAT3 meshAutoColor;

		uint32_t baseColorIndex;
		DX::XMFLOAT3 baseColorFactor;

		uint32_t normalIndex;
		float normalScale;

		uint32_t metalRoughIndex;
		float roughnessFactor;
		float metallicFactor;

		uint32_t aoIndex;
		float aoStrength;

		uint32_t emissiveIndex;
		DX::XMFLOAT3 emissiveFactor;
	};

	/**
	 * @brief Constant buffer updated when a Phong material is changed
	 * @note Is sent as an array.
	 */
	struct perPhongMaterialChange {
		DX::XMFLOAT3 ambientFactor;

		DX::XMFLOAT3 diffuseFactor;
		uint32_t diffuseIndex;

		DX::XMFLOAT3 specularFactor;
		uint32_t specularIndex;

		float shininess;
	};

	/**
	 * @brief Constant buffer updated when a light is changed
	 * @note Is sent as an array.
	 */
	struct perLightChange {
		DX::XMFLOAT3 color;
		float intensity;
		DX::XMFLOAT3 position;
		float spotAngle;
		DX::XMFLOAT3 direction;
		float attenuationRadius;
	};
}
