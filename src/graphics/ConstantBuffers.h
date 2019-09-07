#pragma once

/**
 * @brief Structure for constant buffers used by shaders and updated on CPU side in render system
 */
namespace cb {
	struct Camera {
		XMFLOAT4X4 matViewProj;
		XMFLOAT4X4 matGeo;
	};

	struct Lights {
		XMFLOAT3 strength;
		float falloffStart;		// point and spot light only
		XMFLOAT3 direction;		// directional and spot light only
		float falloffEnd;		// point and spot light only
		XMFLOAT3 position;		// point light only
		float spotPower;		// spot light only
	};

	struct CookTorranceMaterials {
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
}
