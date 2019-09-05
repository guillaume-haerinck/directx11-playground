#pragma once

namespace comp {

	enum SamplerSlot {
		ANISOTROPIC_WRAP = 0,
		LINEAR_CLAMP
	};

	/**
	 * @brief Texture sampler
	 */
	struct Sampler {
		Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
		unsigned int slot;
	};
}
